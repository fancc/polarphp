//===--- Parameter.cpp - Functions & closures parameters ------------------===//
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
// This file defines the Parameter class, the ParameterList class and support
// logic.
//
//===----------------------------------------------------------------------===//

#include "polarphp/ast/ParameterList.h"
#include "polarphp/ast/AstContext.h"
#include "polarphp/ast/Expr.h"
#include "polarphp/ast/Types.h"

namespace polar::ast {

/// TODO: unique and reuse the () parameter list in AstContext, it is common to
/// many methods.  Other parameter lists cannot be uniqued because the decls
/// within them are always different anyway (they have different DeclContext's).
ParameterList *
ParameterList::create(const AstContext &C, SourceLoc LParenLoc,
                      ArrayRef<ParamDecl *> params, SourceLoc RParenLoc) {
   assert(LParenLoc.isValid() == RParenLoc.isValid() &&
          "Either both paren locs are valid or neither are");

   auto byteSize = totalSizeToAlloc<ParamDecl *>(params.size());
   auto rawMem = C.Allocate(byteSize, alignof(ParameterList));

   //  Placement initialize the ParameterList and the Parameter's.
   auto PL = ::new(rawMem) ParameterList(LParenLoc, params.size(), RParenLoc);

   std::uninitialized_copy(params.begin(), params.end(), PL->getArray().begin());

   return PL;
}

/// Change the DeclContext of any contained parameters to the specified
/// DeclContext.
void ParameterList::setDeclContextOfParamDecls(DeclContext *DC) {
   for (auto P : *this)
      P->setDeclContext(DC);
}

/// Make a duplicate copy of this parameter list.  This allocates copies of
/// the ParamDecls, so they can be reparented into a new DeclContext.
ParameterList *ParameterList::clone(const AstContext &C,
                                    OptionSet<CloneFlags> options) const {
   // If this list is empty, don't actually bother with a copy.
   if (size() == 0)
      return const_cast<ParameterList *>(this);

   SmallVector<ParamDecl *, 8> params(begin(), end());

   // Remap the ParamDecls inside of the ParameterList.
   for (auto &decl : params) {
      bool hadDefaultArgument =
         decl->getDefaultArgumentKind() == DefaultArgumentKind::Normal;

      decl = ParamDecl::cloneWithoutType(C, decl);
      if (options & Implicit)
         decl->setImplicit();

      // If the argument isn't named, and we're cloning for an inherited
      // constructor, give the parameter a name so that silgen will produce a
      // value for it.
      if (decl->getName().empty() && (options & Inherited))
         decl->setName(C.getIdentifier("argument"));

      // If we're inheriting a default argument, mark it as such.
      // FIXME: Figure out how to clone default arguments as well.
      if (hadDefaultArgument) {
         if (options & Inherited)
            decl->setDefaultArgumentKind(DefaultArgumentKind::Inherited);
         else
            decl->setDefaultArgumentKind(DefaultArgumentKind::None);
      }
   }

   return create(C, params);
}

void ParameterList::getParams(
   SmallVectorImpl<AnyFunctionType::Param> &params) const {
   for (auto P : *this)
      params.push_back(P->toFunctionParam());
}


/// Return the full source range of this parameter list.
SourceRange ParameterList::getSourceRange() const {
   // If we have locations for the parens, then they define our range.
   if (LParenLoc.isValid())
      return {LParenLoc, RParenLoc};

   // Otherwise, try the first and last parameter.
   if (size() != 0) {
      auto Start = get(0)->getStartLoc();
      auto End = getArray().back()->getEndLoc();
      if (Start.isValid() && End.isValid())
         return {Start, End};
   }

   return SourceRange();
}

} // polar::ast