//===--- PassPipeline.def -------------------------------------------------===//
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
///
/// Metaprogramming for Pass Pipelines
///
//===----------------------------------------------------------------------===//

/// PASSPIPELINE(NAME, DESCRIPTION)
///
///   A pipeline with Name NAME and description DESCRIPTION. PassPipelinePlan
///   constructor is assumed to take a PILOptions value.
#ifndef PASSPIPELINE
#define PASSPIPELINE(NAME, DESCRIPTION)
#endif

PASSPIPELINE(Diagnostic, "Guaranteed Passes")
PASSPIPELINE(OwnershipEliminator, "Utility pass to just run the ownership eliminator pass")
PASSPIPELINE(PILOptPrepare, "Passes that prepare PIL for -O")
PASSPIPELINE(Performance, "Passes run at -O")
PASSPIPELINE(Onone, "Passes run at -Onone")
PASSPIPELINE(InstCount, "Utility pipeline to just run the inst count pass")
PASSPIPELINE(Lowering, "PIL Address Lowering")
PASSPIPELINE(IRGenPrepare, "Pipeline to run during IRGen")
PASSPIPELINE(SerializePIL, "Utility pipeline that just runs SerializePILPass ")

#undef PASSPIPELINE_WITH_OPTIONS
#undef PASSPIPELINE