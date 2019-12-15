//===--- FormalLinkage.h - Formal linkage of types and decls ----*- C++ -*-===//
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

#ifndef POLARPHP_PIL_FORMALLINKAGE_H
#define POLARPHP_PIL_FORMALLINKAGE_H

namespace polar::pil {

class CanType;
class RootInterfaceConformance;
class ValueDecl;
enum class PILLinkage : unsigned char;
enum ForDefinition_t : bool;

/// Formal linkage is a property of types and declarations that
/// informs, but is not completely equivalent to, the linkage of
/// symbols corresponding to those types and declarations.
enum class FormalLinkage {
   /// This entity is visible in multiple Swift modules and has a
   /// unique file that is known to define it.
      PublicUnique,

   /// This entity is visible in multiple Swift modules, but does not
   /// have a unique file that is known to define it.
      PublicNonUnique,

   /// This entity is visible in only a single Swift module and has a
   /// unique file that is known to define it.
      HiddenUnique,

   /// This entity is visible in only a single Swift file. These are by
   /// definition unique.
      Private,
};

FormalLinkage getDeclLinkage(const ValueDecl *decl);
PILLinkage getPILLinkage(FormalLinkage linkage,
                         ForDefinition_t forDefinition);
PILLinkage
getLinkageForInterfaceConformance(const RootInterfaceConformance *C,
                                  ForDefinition_t definition);

} // end swift namespace

#endif