//===--- RawComment.cpp - Extraction of raw comments ----------------------===//
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
/// \file
/// This file implements extraction of raw comments.
///
//===----------------------------------------------------------------------===//

#include "polarphp/ast/RawComment.h"
#include "polarphp/ast/Comment.h"
#include "polarphp/ast/AstContext.h"
#include "polarphp/ast/Decl.h"
#include "polarphp/ast/FileUnit.h"
#include "polarphp/ast/Module.h"
#include "polarphp/ast/PrettyStackTrace.h"
#include "polarphp/ast/Types.h"
#include "polarphp/basic/PrimitiveParsing.h"
#include "polarphp/basic/SourceMgr.h"
#include "polarphp/markup/Markup.h"
#include "polarphp/llparser/Lexer.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/raw_ostream.h"

namespace polar {

using polar::llparser::tok;
using polar::llparser::Lexer;
using polar::llparser::LexerMode;
using polar::llparser::HashbangMode;
using polar::llparser::CommentRetentionMode;
using polar::llparser::TriviaRetentionMode;

static SingleRawComment::CommentKind getCommentKind(StringRef Comment) {
   assert(Comment.size() >= 2);
   assert(Comment[0] == '/');

   if (Comment[1] == '/') {
      if (Comment.size() < 3)
         return SingleRawComment::CommentKind::OrdinaryLine;

      if (Comment[2] == '/') {
         return SingleRawComment::CommentKind::LineDoc;
      }
      return SingleRawComment::CommentKind::OrdinaryLine;
   } else {
      assert(Comment[1] == '*');
      assert(Comment.size() >= 4);
      if (Comment[2] == '*') {
         return SingleRawComment::CommentKind::BlockDoc;
      }
      return SingleRawComment::CommentKind::OrdinaryBlock;
   }
}

SingleRawComment::SingleRawComment(CharSourceRange Range,
                                   const SourceManager &SourceMgr)
   : Range(Range), RawText(SourceMgr.extractText(Range)),
     Kind(static_cast<unsigned>(getCommentKind(RawText))) {
   auto StartLineAndColumn = SourceMgr.getLineAndColumn(Range.getStart());
   StartLine = StartLineAndColumn.first;
   StartColumn = StartLineAndColumn.second;
   EndLine = SourceMgr.getLineNumber(Range.getEnd());
}

SingleRawComment::SingleRawComment(StringRef RawText, unsigned StartColumn)
   : RawText(RawText), Kind(static_cast<unsigned>(getCommentKind(RawText))),
     StartColumn(StartColumn), StartLine(0), EndLine(0) {}

static void addCommentToList(SmallVectorImpl<SingleRawComment> &Comments,
                             const SingleRawComment &SRC) {
   // TODO: consider producing warnings when we decide not to merge comments.

   if (SRC.isOrdinary()) {
      // Skip gyb comments that are line number markers.
      if (SRC.RawText.startswith("// ###"))
         return;

      Comments.clear();
      return;
   }

   // If this is the first documentation comment, save it (because there isn't
   // anything to merge it with).
   if (Comments.empty()) {
      Comments.push_back(SRC);
      return;
   }

   auto &Last = Comments.back();

   // Merge comments if they are on same or consecutive lines.
   if (Last.EndLine + 1 < SRC.StartLine) {
      Comments.clear();
      return;
   }

   Comments.push_back(SRC);
}

static RawComment toRawComment(AstContext &Context, CharSourceRange Range) {
   if (Range.isInvalid())
      return RawComment();

   auto &SourceMgr = Context.SourceMgr;
   unsigned BufferID = SourceMgr.findBufferContainingLoc(Range.getStart());
   unsigned Offset = SourceMgr.getLocOffsetInBuffer(Range.getStart(), BufferID);
   unsigned EndOffset = SourceMgr.getLocOffsetInBuffer(Range.getEnd(), BufferID);
   LangOptions FakeLangOpts;
   Lexer L(FakeLangOpts, SourceMgr, BufferID, nullptr, LexerMode::Polarphp,
           HashbangMode::Disallowed,
           CommentRetentionMode::ReturnAsTokens,
           TriviaRetentionMode::WithoutTrivia,
           Offset, EndOffset);
   SmallVector<SingleRawComment, 16> Comments;
   Token Tok;
   while (true) {
      L.lex(Tok);
      if (Tok.is(tok::eof))
         break;
      assert(Tok.is(tok::comment));
      addCommentToList(Comments, SingleRawComment(Tok.getRange(), SourceMgr));
   }
   RawComment Result;
   Result.Comments = Context.AllocateCopy(Comments);
   return Result;
}

RawComment Decl::getRawComment() const {
   if (!this->canHaveComment())
      return RawComment();

   // Check the cache in AstContext.
   auto &Context = getAstContext();
   if (Optional<RawComment> RC = Context.getRawComment(this))
      return RC.getValue();

   // Check the declaration itself.
   if (auto *Attr = getAttrs().getAttribute<RawDocCommentAttr>()) {
      RawComment Result = toRawComment(Context, Attr->getCommentRange());
      Context.setRawComment(this, Result);
      return Result;
   }

   // Ask the parent module.
   if (auto *Unit =
      dyn_cast<FileUnit>(this->getDeclContext()->getModuleScopeContext())) {
      if (Optional<CommentInfo> C = Unit->getCommentForDecl(this)) {
         polar::markup::MarkupContext MC;
         Context.setBriefComment(this, C->Brief);
         Context.setRawComment(this, C->Raw);
         return C->Raw;
      }
   }

   // Give up.
   return RawComment();
}

static const Decl *getGroupDecl(const Decl *D) {
   auto GroupD = D;

   // Extensions always exist in the same group with the nominal.
   if (auto ED = dyn_cast_or_null<ExtensionDecl>(D->getDeclContext()->
      getInnermostTypeContext())) {
      if (auto ExtNominal = ED->getExtendedNominal())
         GroupD = ExtNominal;
   }
   return GroupD;
}

Optional<StringRef> Decl::getGroupName() const {
   if (hasClangNode())
      return None;
   if (auto GroupD = getGroupDecl(this)) {
      // We can only get group information from deserialized module files.
      if (auto *Unit =
         dyn_cast<FileUnit>(GroupD->getDeclContext()->getModuleScopeContext())) {
         return Unit->getGroupNameForDecl(GroupD);
      }
   }
   return None;
}

Optional<StringRef> Decl::getSourceFileName() const {
   if (hasClangNode())
      return None;
   if (auto GroupD = getGroupDecl(this)) {
      // We can only get group information from deserialized module files.
      if (auto *Unit =
         dyn_cast<FileUnit>(GroupD->getDeclContext()->getModuleScopeContext())) {
         return Unit->getSourceFileNameForDecl(GroupD);
      }
   }
   return None;
}

Optional<unsigned> Decl::getSourceOrder() const {
   if (hasClangNode())
      return None;
   // We can only get source orders from deserialized module files.
   if (auto *Unit =
      dyn_cast<FileUnit>(this->getDeclContext()->getModuleScopeContext())) {
      return Unit->getSourceOrderForDecl(this);
   }
   return None;
}

CharSourceRange RawComment::getCharSourceRange() {
   if (this->isEmpty()) {
      return CharSourceRange();
   }

   auto Start = this->Comments.front().Range.getStart();
   if (Start.isInvalid()) {
      return CharSourceRange();
   }
   auto End = this->Comments.back().Range.getEnd();
   auto Length = static_cast<const char *>(End.getOpaquePointerValue()) -
                 static_cast<const char *>(Start.getOpaquePointerValue());
   return CharSourceRange(Start, Length);
}

} // polar