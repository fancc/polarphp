//===--- Passes.def - Swift PILPass Metaprogramming -------------*- C++ -*-===//
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
// This file defines macros used for macro-metaprogramming with PILPasses.
//
//===----------------------------------------------------------------------===//

/// PASS(Id, Tag, Description)
///   Id is a pass "identifier", used for its enum case, PassKind::Id,
///   and type name, as returned by the global function swift::create##Id().
///
///   Tag identifies the pass as a command-line compatible option string.
///
///   Description is a short description of the pass.
///
///   Id and Tag are unique identifiers which may be used in test
///   cases and tools to specify a pass by string. Different tools simply prefer
///   different identifier formats. Changing any of one these strings may change
///   the functionality of some tests.
///
///   This macro must be defined by the includer.
#ifndef PASS
#error "Macro must be defined by includer"
#endif

/// IRGEN_PASS(Id, Tag, Description)
///   This macro follows the same conventions as PASS(Id, Tag, Description),
///   but is used for IRGen passes which are built outside of the
///   PILOptimizer library.
///
///   An IRGen pass is created by IRGen and needs to be registered with the pass
///   manager dynamically.
#ifndef IRGEN_PASS
#define IRGEN_PASS(Id, Tag, Description) PASS(Id, Tag, Description)
#endif

/// PASS_RANGE(RANGE_ID, START, END)
///   Pass IDs between PassKind::START and PassKind::END, inclusive,
///   fall within the set known as
#ifndef PASS_RANGE
#define PASS_RANGE(Id, First, Last)
#endif

PASS(AADumper, "aa-dump",
     "Dump Alias Analysis over all Pairs")
PASS(ABCOpt, "abcopts",
     "Array Bounds Check Optimization")
PASS(AccessEnforcementDom, "access-enforcement-dom",
     "Remove dominated access checks with no nested conflict")
PASS(AccessEnforcementOpts, "access-enforcement-opts",
     "Access Enforcement Optimization")
PASS(AccessEnforcementReleaseSinking, "access-enforcement-release",
     "Access Enforcement Release Sinking")
PASS(AccessEnforcementSelection, "access-enforcement-selection",
     "Access Enforcement Selection")
PASS(AccessEnforcementWMO, "access-enforcement-wmo",
     "Access Enforcement Whole Module Optimization")
PASS(CrossModuleSerializationSetup, "cross-module-serialization-setup",
     "Setup serialization flags for cross-module optimization")
PASS(AccessSummaryDumper, "access-summary-dump",
     "Dump Address Parameter Access Summary")
PASS(AccessedStorageDumper, "accessed-storage-dump",
     "Dump Accessed Storage Summary")
PASS(AccessMarkerElimination, "access-marker-elim",
     "Access Marker Elimination.")
PASS(AddressLowering, "address-lowering",
     "PIL Address Lowering")
PASS(AllocBoxToStack, "allocbox-to-stack",
     "Stack Promotion of Box Objects")
IRGEN_PASS(AllocStackHoisting, "alloc-stack-hoisting",
           "PIL alloc_stack Hoisting")
PASS(ArrayCountPropagation, "array-count-propagation",
     "Array Count Propagation")
PASS(ArrayElementPropagation, "array-element-propagation",
     "Array Element Propagation")
PASS(AssumeSingleThreaded, "sil-assume-single-threaded",
     "Assume Single-Threaded Environment")
PASS(BasicInstructionPropertyDumper, "basic-instruction-property-dump",
     "Print PIL Instruction MemBehavior and ReleaseBehavior Information")
PASS(BasicCalleePrinter, "basic-callee-printer",
     "Print Basic Callee Analysis for Testing")
PASS(CFGPrinter, "view-cfg",
     "View Function CFGs")
PASS(COWArrayOpts, "cowarray-opt",
     "COW Array Optimization")
PASS(CSE, "cse",
     "Common Subexpression Elimination")
PASS(CallerAnalysisPrinter, "caller-analysis-printer",
     "Print Caller Analysis for Testing")
PASS(CapturePromotion, "capture-promotion",
     "Capture Promotion to Eliminate Escaping Boxes")
PASS(CapturePropagation, "capture-prop",
     "Captured Constant Propagation")
PASS(ClosureSpecializer, "closure-specialize",
     "Closure Specialization on Constant Function Arguments")
PASS(ClosureLifetimeFixup, "closure-lifetime-fixup",
     "Closure Lifetime Fixup")
PASS(CodeSinking, "code-sinking",
     "Code Sinking")
PASS(ComputeDominanceInfo, "compute-dominance-info",
     "Compute Dominance Information for Testing")
PASS(ComputeLoopInfo, "compute-loop-info",
     "Compute Loop Information for Testing")
PASS(ConditionForwarding, "condition-forwarding",
     "Conditional Branch Forwarding to Fold PIL switch_enum")
PASS(ConstantEvaluatorTester, "test-constant-evaluator",
      "Test constant evaluator")
PASS(ConstantEvaluableSubsetChecker, "test-constant-evaluable-subset",
     "Test polarphp code snippets expected to be constant evaluable")
PASS(CopyForwarding, "copy-forwarding",
     "Copy Forwarding to Remove Redundant Copies")
PASS(CopyPropagation, "copy-propagation",
     "Copy propagation to Remove Redundant SSA Copies")
PASS(EpilogueARCMatcherDumper, "sil-epilogue-arc-dumper",
     "Print Epilogue retains of Returned Values and Argument releases")
PASS(EpilogueRetainReleaseMatcherDumper, "sil-epilogue-retain-release-dumper",
     "Print Epilogue retains of Returned Values and Argument releases")
PASS(RedundantOverflowCheckRemoval, "remove-redundant-overflow-checks",
     "Redundant Overflow Check Removal")
PASS(DCE, "dce",
     "Dead Code Elimination")
PASS(DeadArgSignatureOpt, "dead-arg-signature-opt",
     "Dead Argument Elimination via Function Specialization")
PASS(DeadFunctionElimination, "sil-deadfuncelim",
     "Dead Function Elimination")
PASS(DeadObjectElimination, "deadobject-elim",
     "Dead Object Elimination for Classes with Trivial Destruction")
PASS(DefiniteInitialization, "definite-init",
     "Definite Initialization for Diagnostics")
PASS(DestroyHoisting, "destroy-hoisting",
     "Hoisting of value destroys")
PASS(Devirtualizer, "devirtualizer",
     "Indirect Call Devirtualization")
PASS(DiagnoseInfiniteRecursion, "diagnose-infinite-recursion",
     "Diagnose Infinitely-Recursive Code")
PASS(DiagnoseInvalidEscapingCaptures, "diagnose-invalid-escaping-captures",
     "Diagnose Invalid Escaping Captures")
PASS(DiagnoseStaticExclusivity, "diagnose-static-exclusivity",
     "Static Enforcement of Law of Exclusivity")
PASS(DiagnoseUnreachable, "diagnose-unreachable",
     "Diagnose Unreachable Code")
PASS(DiagnosticConstantPropagation, "diagnostic-constant-propagation",
     "Constants Propagation for Diagnostics")
PASS(EagerSpecializer, "eager-specializer",
     "Eager Specialization via @_specialize")
PASS(EarlyCodeMotion, "early-codemotion",
     "Early Code Motion without Release Hoisting")
PASS(EarlyInliner, "early-inline",
     "Early Inlining Preserving Semantic Functions")
PASS(EmitDFDiagnostics, "dataflow-diagnostics",
     "Emit PIL Diagnostics")
PASS(EscapeAnalysisDumper, "escapes-dump",
     "Dump Escape Analysis Results")
PASS(FunctionOrderPrinter, "function-order-printer",
     "Print Function Order for Testing")
PASS(FunctionSignatureOpts, "function-signature-opts",
     "Function Signature Optimization")
PASS(ARCSequenceOpts, "arc-sequence-opts",
     "ARC Sequence Optimization")
PASS(ARCLoopOpts, "arc-loop-opts",
     "ARC Loop Optimization")
PASS(EarlyRedundantLoadElimination, "early-redundant-load-elim",
     "Early Redundant Load Elimination")
PASS(RedundantLoadElimination, "redundant-load-elim",
     "Redundant Load Elimination")
PASS(DeadStoreElimination, "dead-store-elim",
     "Dead Store Elimination")
PASS(GenericSpecializer, "generic-specializer",
     "Generic Function Specialization on Static Types")
PASS(ExistentialSpecializer, "existential-specializer",
     "Existential Specializer")
PASS(GlobalOpt, "global-opt",
     "PIL Global Optimization")
PASS(GlobalPropertyOpt, "global-property-opt",
     "Global Property Optimization")
PASS(GuaranteedARCOpts, "guaranteed-arc-opts",
     "Guaranteed ARC Optimization")
PASS(HighLevelCSE, "high-level-cse",
     "Common Subexpression Elimination on High-Level PIL")
PASS(HighLevelLICM, "high-level-licm",
     "Loop Invariant Code Motion in High-Level PIL")
PASS(IVInfoPrinter, "iv-info-printer",
     "Print Induction Variable Information for Testing")
PASS(InstCount, "inst-count",
     "Record PIL Instruction, Block, and Function Counts as LLVM Statistics")
PASS(JumpThreadSimplifyCFG, "jumpthread-simplify-cfg",
     "Simplify CFG via Jump Threading")
PASS(LetPropertiesOpt, "let-properties-opt",
     "Let Property Optimization")
PASS(LICM, "licm",
     "Loop Invariant Code Motion")
PASS(LateCodeMotion, "late-codemotion",
     "Late Code Motion with Release Hoisting")
PASS(LateInliner, "late-inline",
     "Late Function Inlining")
PASS(LoopCanonicalizer, "loop-canonicalizer",
     "Loop Canonicalization")
PASS(LoopInfoPrinter, "loop-info-printer",
     "Print Loop Information for Testing")
PASS(LoopRegionViewText, "loop-region-view-text",
     "Print Loop Region Information for Testing")
PASS(LoopRegionViewCFG, "loop-region-view-cfg",
     "View Loop Region CFG")
PASS(LoopRotate, "loop-rotate",
     "Loop Rotation")
PASS(LoopUnroll, "loop-unroll",
     "Loop Unrolling")
PASS(LowerAggregateInstrs, "lower-aggregate-instrs",
     "Lower Aggregate PIL Instructions to Multiple Scalar Operations")
PASS(MandatoryInlining, "mandatory-inlining",
     "Mandatory Inlining of Transparent Functions")
PASS(Mem2Reg, "mem2reg",
     "Memory to SSA Value Conversion to Remove Stack Allocation")
PASS(MemBehaviorDumper, "mem-behavior-dump",
     "Print PIL Instruction MemBehavior from Alias Analysis over all Pairs")
PASS(LSLocationPrinter, "lslocation-dump",
     "Print Load-Store Location Results Covering all Accesses")
PASS(MergeCondFails, "merge-cond_fails",
     "Merge PIL cond_fail to Eliminate Redundant Overflow Checks")
PASS(MoveCondFailToPreds, "move-cond-fail-to-preds",
     "Move PIL cond_fail by Hoisting Checks")
PASS(NoReturnFolding, "noreturn-folding",
     "Prune Control Flow at No-Return Calls Using PIL unreachable")
PASS(ObjectOutliner, "object-outliner",
     "Outlining of Global Objects")
PASS(Outliner, "outliner",
     "Function Outlining Optimization")
PASS(OwnershipModelEliminator, "ownership-model-eliminator",
     "Eliminate Ownership Annotation of PIL")
PASS(NonTransparentFunctionOwnershipModelEliminator,
     "non-transparent-func-ownership-model-eliminator",
     "Eliminate Ownership Annotations from non-transparent PIL Functions")
PASS(RCIdentityDumper, "rc-id-dumper",
     "Print Reference Count Identities")
PASS(PerfInliner, "inline",
     "Performance Function Inlining")
PASS(PerformanceConstantPropagation, "performance-constant-propagation",
     "Constant Propagation for Performance without Diagnostics")
PASS(PredictableMemoryAccessOptimizations, "predictable-memaccess-opts",
     "Predictable Memory Access Optimizations for Diagnostics")
PASS(PredictableDeadAllocationElimination, "predictable-deadalloc-elim",
     "Eliminate dead temporary allocations after diagnostics")
PASS(ReleaseDevirtualizer, "release-devirtualizer",
     "PIL release Devirtualization")
PASS(RetainSinking, "retain-sinking",
     "PIL retain Sinking")
PASS(ReleaseHoisting, "release-hoisting",
     "PIL release Hoisting")
PASS(LateReleaseHoisting, "late-release-hoisting",
     "Late PIL release Hoisting Preserving Epilogues")
IRGEN_PASS(LoadableByAddress, "loadable-address",
     "PIL Large Loadable type by-address lowering.")
PASS(MandatoryPILLinker, "mandatory-linker",
     "Deserialize all referenced PIL functions that are shared or transparent")
PASS(PerformancePILLinker, "performance-linker",
     "Deserialize all referenced PIL functions")
PASS(RawPILInstLowering, "raw-sil-inst-lowering",
     "Lower all raw PIL instructions to canonical equivalents.")
PASS(TempRValueOpt, "temp-rvalue-opt",
     "Remove short-lived immutable temporary copies")
PASS(SideEffectsDumper, "side-effects-dump",
     "Print Side-Effect Information for all Functions")
PASS(IRGenPrepare, "irgen-prepare",
     "Cleanup PIL in preparation for IRGen")
PASS(PILGenCleanup, "silgen-cleanup",
     "Cleanup PIL in preparation for diagnostics")
PASS(PILCombine, "sil-combine",
     "Combine PIL Instructions via Peephole Optimization")
PASS(PILDebugInfoGenerator, "sil-debuginfo-gen",
     "Generate Debug Information with Source Locations into Textual PIL")
PASS(SROA, "sroa",
     "Scalar Replacement of Aggregate Stack Objects")
PASS(SROABBArgs, "sroa-bb-args",
     "Scalar Replacement of Aggregate PIL Block Arguments")
PASS(SimplifyBBArgs, "simplify-bb-args",
     "PIL Block Argument Simplification")
PASS(SimplifyCFG, "simplify-cfg",
     "PIL CFG Simplification")
PASS(SpeculativeDevirtualization, "specdevirt",
     "Speculative Devirtualization via Guarded Calls")
PASS(SplitAllCriticalEdges, "split-critical-edges",
     "Split all Critical Edges in the PIL CFG")
PASS(SplitNonCondBrCriticalEdges, "split-non-cond_br-critical-edges",
     "Split all Critical Edges not from PIL cond_br")
PASS(StackPromotion, "stack-promotion",
     "Stack Promotion of Class Objects")
PASS(StripDebugInfo, "strip-debug-info",
     "Strip Debug Information")
PASS(TypePHPArrayPropertyOpt, "array-property-opt",
     "Loop Specialization for Array Properties")
PASS(UnsafeGuaranteedPeephole, "unsafe-guaranteed-peephole",
     "PIL retain/release Peephole Removal for Builtin.unsafeGuaranteed")
PASS(UsePrespecialized, "use-prespecialized",
     "Use Pre-Specialized Functions")
PASS(OwnershipDumper, "ownership-dumper",
     "Print Ownership information for Testing")
PASS(SemanticARCOpts, "semantic-arc-opts",
     "Semantic ARC Optimization")
PASS(MarkUninitializedFixup, "mark-uninitialized-fixup",
     "Temporary pass for staging in mark_uninitialized changes.")
PASS(SimplifyUnreachableContainingBlocks, "simplify-unreachable-containing-blocks",
     "Utility pass. Removes all non-term insts from blocks with unreachable terms")
PASS(SerializePILPass, "serialize-sil",
     "Utility pass. Serializes the current PILModule")
PASS(NonInlinableFunctionSkippingChecker, "check-non-inlinable-function-skipping",
     "Utility pass to ensure -experimental-skip-non-inlinable-function-bodies "
     "skips everything it should")
PASS(YieldOnceCheck, "yield-once-check",
    "Check correct usage of yields in yield-once coroutines")
PASS(OSLogOptimization, "os-log-optimization", "Optimize os log calls")
PASS(MandatoryCombine, "mandatory-combine",
    "Perform mandatory peephole combines")
PASS(BugReducerTester, "bug-reducer-tester",
     "sil-bug-reducer Tool Testing by Asserting on a Sentinel Function")
PASS_RANGE(AllPasses, AADumper, BugReducerTester)

#undef IRGEN_PASS
#undef PASS
#undef PASS_RANGE
