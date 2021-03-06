//===--- PILGenForeignError.cpp - Error-handling code emission ------------===//
//
// This source file is part of the Swift.org open source project
//
// Copyright (c) 2014 - 2017 Apple Inc. and the Swift project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See https://swift.org/LICENSE.txt for license information
// See https://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
//
//===----------------------------------------------------------------------===//

#include "polarphp/pil/gen/PILGen.h"
#include "polarphp/pil/gen/PILGenFunction.h"
#include "polarphp/pil/gen/AstVisitor.h"
#include "polarphp/pil/gen/LValue.h"
#include "polarphp/pil/gen/RValue.h"
#include "polarphp/pil/gen/Scope.h"
#include "polarphp/pil/lang/PILArgument.h"
#include "polarphp/pil/lang/PILUndef.h"
#include "polarphp/ast/ForeignErrorConvention.h"
#include "polarphp/ast/DiagnosticsPIL.h"

using namespace polar;
using namespace lowering;

namespace {
/// An abstract interface for producing bridged errors.
struct BridgedErrorSource {
   virtual ~BridgedErrorSource() = default;
   virtual PILValue emitBridged(PILGenFunction &SGF, PILLocation loc,
                                CanType bridgedError) const = 0;
   virtual void emitRelease(PILGenFunction &SGF, PILLocation loc) const = 0;
};
} // end anonymous namespace

/// Emit a store of a native error to the foreign-error slot.
static void emitStoreToForeignErrorSlot(PILGenFunction &SGF,
                                        PILLocation loc,
                                        PILValue foreignErrorSlot,
                                        const BridgedErrorSource &errorSrc) {
   AstContext &ctx = SGF.getAstContext();

   // The foreign error slot has type SomePointer<SomeError?>,
   // or possibly an optional thereof.

   // If the pointer itself is optional, we need to branch based on
   // whether it's really there.
   if (PILType errorPtrObjectTy =
      foreignErrorSlot->getType().getOptionalObjectType()) {
      PILBasicBlock *contBB = SGF.createBasicBlock();
      PILBasicBlock *noSlotBB = SGF.createBasicBlock();
      PILBasicBlock *hasSlotBB = SGF.createBasicBlock();
      SGF.B.createSwitchEnum(loc, foreignErrorSlot, nullptr,
                             { { ctx.getOptionalSomeDecl(), hasSlotBB },
                               { ctx.getOptionalNoneDecl(), noSlotBB } });

      // If we have the slot, emit a store to it.
      SGF.B.emitBlock(hasSlotBB);
      PILValue slot = hasSlotBB->createPhiArgument(errorPtrObjectTy,
                                                   ValueOwnershipKind::Owned);
      emitStoreToForeignErrorSlot(SGF, loc, slot, errorSrc);
      SGF.B.createBranch(loc, contBB);

      // Otherwise, just release the error.
      SGF.B.emitBlock(noSlotBB);
      errorSrc.emitRelease(SGF, loc);
      SGF.B.createBranch(loc, contBB);

      // Continue.
      SGF.B.emitBlock(contBB);
      return;
   }

   // Okay, break down the components of SomePointer<SomeError?>.
   // TODO: this should really be an unlowered Ast type?
   auto bridgedErrorPtrType = foreignErrorSlot->getType().getAstType();

   PointerTypeKind ptrKind;
   CanType bridgedErrorProto =
      CanType(bridgedErrorPtrType->getAnyPointerElementType(ptrKind));

   FullExpr scope(SGF.Cleanups, CleanupLocation::get(loc));
   FormalEvaluationScope writebacks(SGF);

   // Convert the error to a bridged form.
   PILValue bridgedError = errorSrc.emitBridged(SGF, loc, bridgedErrorProto);

   // Store to the "pointee" property.
   // If we can't find it, diagnose and then just don't store anything.
   VarDecl *pointeeProperty = ctx.getPointerPointeePropertyDecl(ptrKind);
   if (!pointeeProperty) {
      SGF.SGM.diagnose(loc, diag::could_not_find_pointer_pointee_property,
                       bridgedErrorPtrType);
      return;
   }

   // Otherwise, do a normal assignment.
   LValue lvalue =
      SGF.emitPropertyLValue(loc, ManagedValue::forUnmanaged(foreignErrorSlot),
                             bridgedErrorPtrType, pointeeProperty,
                             LValueOptions(),
                             SGFAccessKind::Write,
                             AccessSemantics::Ordinary);
   RValue rvalue(SGF, loc, bridgedErrorProto,
                 SGF.emitManagedRValueWithCleanup(bridgedError));
   SGF.emitAssignToLValue(loc, std::move(rvalue), std::move(lvalue));
}

/// Emit a value of a certain integer-like type.
static PILValue emitIntValue(PILGenFunction &SGF, PILLocation loc,
                             PILType type, unsigned value) {
   if (auto structDecl = type.getStructOrBoundGenericStruct()) {
      auto properties = structDecl->getStoredProperties();
      assert(properties.size() == 1);
      PILType fieldType = type.getFieldType(properties[0], SGF.SGM.M,
                                            SGF.getTypeExpansionContext());
      PILValue fieldValue = emitIntValue(SGF, loc, fieldType, value);
      return SGF.B.createStruct(loc, type, fieldValue);
   }

   assert(type.is<BuiltinIntegerType>());
   return SGF.B.createIntegerLiteral(loc, type, value);
}

namespace {
/// An error source that bridges a native error.
class EpilogErrorSource : public BridgedErrorSource {
   PILValue NativeError;
public:
   EpilogErrorSource(PILValue nativeError) : NativeError(nativeError) {}

   PILValue emitBridged(PILGenFunction &SGF, PILLocation loc,
                        CanType bridgedErrorProto) const override {
      auto nativeErrorType = NativeError->getType().getAstType();
      assert(nativeErrorType == SGF.SGM.getAstContext().getExceptionType());

      PILValue bridgedError = SGF.emitNativeToBridgedError(loc,
                                                           SGF.emitManagedRValueWithCleanup(NativeError),
                                                           nativeErrorType,
                                                           bridgedErrorProto).forward(SGF);
      return bridgedError;
   }

   void emitRelease(PILGenFunction &SGF, PILLocation loc) const override {
      SGF.B.emitDestroyValueOperation(loc, NativeError);
   }
};

/// An error source that produces nil errors.
class NilErrorSource : public BridgedErrorSource {
public:
   PILValue emitBridged(PILGenFunction &SGF, PILLocation loc,
                        CanType bridgedError) const override {
      PILType optTy = SGF.getLoweredType(bridgedError);
      return SGF.B.createOptionalNone(loc, optTy);
   }

   void emitRelease(PILGenFunction &SGF, PILLocation loc) const override {
   }
};
} // end anonymous namespace

/// Given that we are throwing a native error, turn it into a bridged
/// error, dispose of it in the correct way, and create the appropriate
/// normal return value for the given foreign-error convention.
PILValue PILGenFunction::
emitBridgeErrorForForeignError(PILLocation loc,
                               PILValue nativeError,
                               PILType bridgedResultType,
                               PILValue foreignErrorSlot,
                               const ForeignErrorConvention &foreignError) {
   FullExpr scope(Cleanups, CleanupLocation::get(loc));

   // Store the error to the foreign error slot.
   emitStoreToForeignErrorSlot(*this, loc, foreignErrorSlot,
                               EpilogErrorSource(nativeError));

   switch (foreignError.getKind()) {
      case ForeignErrorConvention::ZeroResult:
         return emitIntValue(*this, loc, bridgedResultType, 0);
      case ForeignErrorConvention::ZeroPreservedResult:
         return emitIntValue(*this, loc, bridgedResultType, 0);
      case ForeignErrorConvention::NonZeroResult:
         return emitIntValue(*this, loc, bridgedResultType, 1);
      case ForeignErrorConvention::NilResult:
         return B.createOptionalNone(loc, bridgedResultType);
      case ForeignErrorConvention::NonNilError:
         return PILUndef::get(bridgedResultType, F);
   }
   llvm_unreachable("bad foreign error convention kind");
}

/// Given that we are returning a normal value, convert it to a
/// bridged representation and set up a return value according to the
/// given foreign-error convention.
PILValue PILGenFunction::
emitBridgeReturnValueForForeignError(PILLocation loc,
                                     PILValue result,
                                     CanType formalNativeType,
                                     CanType formalBridgedType,
                                     PILType bridgedType,
                                     PILValue foreignErrorSlot,
                                     const ForeignErrorConvention &foreignError) {
   FullExpr scope(Cleanups, CleanupLocation::get(loc));

   switch (foreignError.getKind()) {
      // If an error is signalled by a zero result, return non-zero.
      case ForeignErrorConvention::ZeroResult:
         return emitIntValue(*this, loc, bridgedType, 1);

         // If an error is signalled by a non-zero result, return zero.
      case ForeignErrorConvention::NonZeroResult:
         return emitIntValue(*this, loc, bridgedType, 0);

         // If an error is signalled by a zero result, but we've preserved
         // the rest of the return value, then just return the normal
         // result, assuming (hoping!) that it isn't zero.
      case ForeignErrorConvention::ZeroPreservedResult:
         return result;

         // If an error is signalled by a nil result, inject a non-nil result.
      case ForeignErrorConvention::NilResult: {
         ManagedValue bridgedResult = emitNativeToBridgedValue(
            loc, emitManagedRValueWithCleanup(result), formalNativeType,
            formalBridgedType, bridgedType.getOptionalObjectType());

         auto someResult =
            B.createOptionalSome(loc, bridgedResult.forward(*this), bridgedType);
         return someResult;
      }

         // If an error is signalled by a non-nil error, be sure to store a
         // nil error there.
      case ForeignErrorConvention::NonNilError: {
         // Store nil to the foreign error slot.
         emitStoreToForeignErrorSlot(*this, loc, foreignErrorSlot, NilErrorSource());

         // The actual result value just needs to be bridged normally.
         ManagedValue bridgedValue =
            emitNativeToBridgedValue(loc, emitManagedRValueWithCleanup(result),
                                     formalNativeType, formalBridgedType,
                                     bridgedType);
         return bridgedValue.forward(*this);
      }
   }
   llvm_unreachable("bad foreign error convention kind");
}

/// Step out of the current control flow to emit a foreign error block,
/// which loads from the error slot and jumps to the error slot.
void PILGenFunction::emitForeignErrorBlock(PILLocation loc,
                                           PILBasicBlock *errorBB,
                                           Optional<ManagedValue> errorSlot) {
   PILGenSavedInsertionPoint savedIP(*this, errorBB,
                                     FunctionSection::Postmatter);
   Scope scope(Cleanups, CleanupLocation::get(loc));

   // Load the error (taking responsibility for it).  In theory, this
   // is happening within conditional code, so we need to be only
   // conditionally claiming the value.  In practice, claiming it
   // unconditionally is fine because we want to assume it's nil in the
   // other path.
   PILValue errorV;
   if (errorSlot.hasValue()) {
      errorV = B.emitLoadValueOperation(loc, errorSlot.getValue().forward(*this),
                                        LoadOwnershipQualifier::Take);
   } else {
      // If we are not provided with an errorSlot value, then we are passed the
      // unwrapped optional error as the argument of the errorBB. This value is
      // passed at +1 meaning that we still need to create a cleanup for errorV.
      errorV = errorBB->getArgument(0);
   }

   ManagedValue error = emitManagedRValueWithCleanup(errorV);

   // Turn the error into an Error value.
   error = scope.popPreservingValue(emitBridgedToNativeError(loc, error));

   // Propagate.
   FullExpr throwScope(Cleanups, CleanupLocation::get(loc));
   emitThrow(loc, error, true);
}

/// Perform a foreign error check by testing whether the call result is zero.
/// The call result is otherwise ignored.
static void
emitResultIsZeroErrorCheck(PILGenFunction &SGF, PILLocation loc,
                           ManagedValue result, ManagedValue errorSlot,
                           bool suppressErrorCheck, bool zeroIsError) {
   // Just ignore the call result if we're suppressing the error check.
   if (suppressErrorCheck) {
      return;
   }

   PILValue resultValue =
      SGF.emitUnwrapIntegerResult(loc, result.getUnmanagedValue());
   auto resultType = resultValue->getType().getAstType();

   if (!resultType->isBuiltinIntegerType(1)) {
      PILValue zero =
         SGF.B.createIntegerLiteral(loc, resultValue->getType(), 0);

      AstContext &ctx = SGF.getAstContext();
      resultValue =
         SGF.B.createBuiltinBinaryFunction(loc,
                                           "cmp_ne",
                                           resultValue->getType(),
                                           PILType::getBuiltinIntegerType(1, ctx),
                                           {resultValue, zero});
   }

   PILBasicBlock *errorBB = SGF.createBasicBlock(FunctionSection::Postmatter);
   PILBasicBlock *contBB = SGF.createBasicBlock();

   if (zeroIsError)
      SGF.B.createCondBranch(loc, resultValue, contBB, errorBB);
   else
      SGF.B.createCondBranch(loc, resultValue, errorBB, contBB);

   SGF.emitForeignErrorBlock(loc, errorBB, errorSlot);

   SGF.B.emitBlock(contBB);
}

/// Perform a foreign error check by testing whether the call result is nil.
static ManagedValue
emitResultIsNilErrorCheck(PILGenFunction &SGF, PILLocation loc,
                          ManagedValue origResult, ManagedValue errorSlot,
                          bool suppressErrorCheck) {
   // Take local ownership of the optional result value.
   PILValue optionalResult = origResult.forward(SGF);

   PILType resultObjectType = optionalResult->getType().getOptionalObjectType();

   AstContext &ctx = SGF.getAstContext();

   // If we're suppressing the check, just do an unchecked take.
   if (suppressErrorCheck) {
      PILValue objectResult =
         SGF.B.createUncheckedEnumData(loc, optionalResult,
                                       ctx.getOptionalSomeDecl());
      return SGF.emitManagedRValueWithCleanup(objectResult);
   }

   // Switch on the optional result.
   PILBasicBlock *errorBB = SGF.createBasicBlock(FunctionSection::Postmatter);
   PILBasicBlock *contBB = SGF.createBasicBlock();
   SGF.B.createSwitchEnum(loc, optionalResult, /*default*/ nullptr,
                          { { ctx.getOptionalSomeDecl(), contBB },
                            { ctx.getOptionalNoneDecl(), errorBB } });

   // Emit the error block.
   SGF.emitForeignErrorBlock(loc, errorBB, errorSlot);

   // In the continuation block, take ownership of the now non-optional
   // result value.
   SGF.B.emitBlock(contBB);
   PILValue objectResult =
      contBB->createPhiArgument(resultObjectType, ValueOwnershipKind::Owned);
   return SGF.emitManagedRValueWithCleanup(objectResult);
}

/// Perform a foreign error check by testing whether the error was nil.
static void
emitErrorIsNonNilErrorCheck(PILGenFunction &SGF, PILLocation loc,
                            ManagedValue errorSlot, bool suppressErrorCheck) {
   // If we're suppressing the check, just don't check.
   if (suppressErrorCheck) return;

   PILValue optionalError = SGF.B.emitLoadValueOperation(
      loc, errorSlot.forward(SGF), LoadOwnershipQualifier::Take);

   AstContext &ctx = SGF.getAstContext();

   // Switch on the optional error.
   PILBasicBlock *errorBB = SGF.createBasicBlock(FunctionSection::Postmatter);
   errorBB->createPhiArgument(optionalError->getType().unwrapOptionalType(),
                              ValueOwnershipKind::Owned);
   PILBasicBlock *contBB = SGF.createBasicBlock();
   SGF.B.createSwitchEnum(loc, optionalError, /*default*/ nullptr,
                          { { ctx.getOptionalSomeDecl(), errorBB },
                            { ctx.getOptionalNoneDecl(), contBB } });

   // Emit the error block. Pass in none for the errorSlot since we have passed
   // in the errorSlot as our BB argument so we can pass ownership correctly. In
   // emitForeignErrorBlock, we will create the appropriate cleanup for the
   // argument.
   SGF.emitForeignErrorBlock(loc, errorBB, None);

   // Return the result.
   SGF.B.emitBlock(contBB);
   return;
}

/// Emit a check for whether a non-native function call produced an
/// error.
///
/// \c results should be left with only values that match the formal
/// direct results of the function.
void
PILGenFunction::emitForeignErrorCheck(PILLocation loc,
                                      SmallVectorImpl<ManagedValue> &results,
                                      ManagedValue errorSlot,
                                      bool suppressErrorCheck,
                                      const ForeignErrorConvention &foreignError) {
   // All of this is autogenerated.
   loc.markAutoGenerated();

   switch (foreignError.getKind()) {
      case ForeignErrorConvention::ZeroPreservedResult:
         assert(results.size() == 1);
         emitResultIsZeroErrorCheck(*this, loc, results[0], errorSlot,
                                    suppressErrorCheck,
            /*zeroIsError*/ true);
         return;
      case ForeignErrorConvention::ZeroResult:
         assert(results.size() == 1);
         emitResultIsZeroErrorCheck(*this, loc, results.pop_back_val(),
                                    errorSlot, suppressErrorCheck,
            /*zeroIsError*/ true);
         return;
      case ForeignErrorConvention::NonZeroResult:
         assert(results.size() == 1);
         emitResultIsZeroErrorCheck(*this, loc, results.pop_back_val(),
                                    errorSlot, suppressErrorCheck,
            /*zeroIsError*/ false);
         return;
      case ForeignErrorConvention::NilResult:
         assert(results.size() == 1);
         results[0] = emitResultIsNilErrorCheck(*this, loc, results[0], errorSlot,
                                                suppressErrorCheck);
         return;
      case ForeignErrorConvention::NonNilError:
         // Leave the direct results alone.
         emitErrorIsNonNilErrorCheck(*this, loc, errorSlot, suppressErrorCheck);
         return;
   }
   llvm_unreachable("bad foreign error convention kind");
}
