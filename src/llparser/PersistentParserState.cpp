//===--- PersistentParserState.cpp - Parser State Implementation ----------===//
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
//  This file implements parser state persistent across multiple parses.
//
//===----------------------------------------------------------------------===//

#include "polarphp/ast/AstContext.h"
#include "polarphp/ast/Decl.h"
#include "polarphp/ast/Expr.h"
#include "polarphp/llparser/PersistentParserState.h"

namespace polar::llparser {

PersistentParserState::PersistentParserState() {}

PersistentParserState::~PersistentParserState() {}

void PersistentParserState::setCodeCompletionDelayedDeclState(
   CodeCompletionDelayedDeclKind Kind, unsigned Flags,
   DeclContext *ParentContext, SourceRange BodyRange, SourceLoc PreviousLoc) {
   assert(!CodeCompletionDelayedDeclStat.get() &&
          "only one decl can be delayed for code completion");
   CodeCompletionDelayedDeclStat.reset(new CodeCompletionDelayedDeclState(
      Kind, Flags, ParentContext, BodyRange, PreviousLoc,
      scopeInfo.saveCurrentScope()));
}

void PersistentParserState::delayDeclList(IterableDeclContext *D) {
   DelayedDeclLists.push_back(D);
}

void PersistentParserState::parseAllDelayedDeclLists() {
   for (auto IDC : DelayedDeclLists)
      IDC->loadAllMembers();
}

} // polar::llparser