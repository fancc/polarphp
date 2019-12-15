//===--- ValueTracking.h - PIL Value Tracking Analysis ----------*- C++ -*-===//
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
//
// This file contains routines which analyze chains of computations.
//
//===----------------------------------------------------------------------===//

#ifndef POLARPHP_PIL_OPTIMIZER_ANALYSIS_VALUETRACKING_H
#define POLARPHP_PIL_OPTIMIZER_ANALYSIS_VALUETRACKING_H

#include "polarphp/pil/lang/InstructionUtils.h"
#include "polarphp/pil/lang/PILArgument.h"
#include "polarphp/pil/lang/PILInstruction.h"

namespace polar::pil {

/// Returns true if \p V is a function argument which may not alias to
/// any other pointer in the function.
///
/// This does not look through any projections. The caller must do that.
bool isExclusiveArgument(PILValue V);

/// Returns true if \p V is a locally allocated object.
///
/// Note: this may look through a single level of indirection (via
/// ref_element_addr) when \p V is the address of a class property. However, it
/// does not look through init/open_existential_addr.
bool pointsToLocalObject(PILValue V);

/// Returns true if \p V is a uniquely identified address or reference. It may
/// be any of:
///
/// - an address projection based on a locally allocated address with no
/// indirection
///
/// - a locally allocated reference, or an address projection based on that
/// reference with one level of indirection (an address into the locally
/// allocated object).
///
/// - an address projection based on an exclusive argument with no levels of
/// indirection (e.g. ref_element_addr, project_box, etc.).
inline bool isUniquelyIdentified(PILValue V) {
   return pointsToLocalObject(V)
          || isExclusiveArgument(getUnderlyingAddressRoot(V));
}

enum class IsZeroKind {
   Zero,
   NotZero,
   Unknown
};

/// Check if the value \p Value is known to be zero, non-zero or unknown.
IsZeroKind isZeroValue(PILValue Value);

/// Checks if a sign bit of a value is known to be set, not set or unknown.
/// Essentially, it is a simple form of a range analysis.
/// This approach is inspired by the corresponding implementation of
/// ComputeSignBit in LLVM's value tracking implementation.
/// It is planned to extend this approach to track all bits of a value.
/// Therefore it can be considered to be the beginning of a range analysis
/// infrastructure for the Swift compiler.
Optional<bool> computeSignBit(PILValue Value);

/// Check if execution of a given builtin instruction can result in overflows.
/// Returns true of an overflow can happen. Otherwise returns false.
bool canOverflow(BuiltinInst *BI);

} // end namespace polar::pil

#endif // POLARPHP_PIL_OPTIMIZER_ANALYSIS_VALUETRACKING_H