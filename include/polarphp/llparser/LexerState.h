//===--- LexerState.h - Lexer State -----------------------------*- C++ -*-===//
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
//  This file defines the LexerState object.
//
//===----------------------------------------------------------------------===//

#ifndef POLARPHP_LLPARSER_LEXERSTATE_H
#define POLARPHP_LLPARSER_LEXERSTATE_H

#include "llvm/ADT/Optional.h"
#include "polarphp/basic/SourceLoc.h"
#include "polarphp/llparser/ParsedTrivia.h"

namespace polar::llparser {
class Lexer;

using polar::basic::SourceLoc;

/// Lexer state can be saved/restored to/from objects of this class.
class LexerState {
public:
   LexerState() {}

   bool isValid() const { return Loc.isValid(); }

   LexerState advance(unsigned Offset) const {
      assert(isValid());
      return LexerState(Loc.getAdvancedLoc(Offset));
   }

private:
   explicit LexerState(SourceLoc Loc) : Loc(Loc) {}
   SourceLoc Loc;
   llvm::Optional<ParsedTrivia> LeadingTrivia;
   friend class Lexer;
};

} // end namespace polar::llparser

#endif
