//===--- RawComment.h - Extraction of raw comments --------------*- C++ -*-===//
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

#ifndef POLARPHP_AST_RAW_COMMENT_H
#define POLARPHP_AST_RAW_COMMENT_H

#include "polarphp/basic/LLVM.h"
#include "polarphp/basic/SourceLoc.h"
#include "polarphp/basic/SourceMgr.h"

namespace polar {

using polar::CharSourceRange;
using polar::SourceManager;

struct SingleRawComment {
   enum class CommentKind {
      OrdinaryLine,  ///< Any normal // comments
      OrdinaryBlock, ///< Any normal /* */ comment
      LineDoc,       ///< \code /// stuff \endcode
      BlockDoc,      ///< \code /** stuff */ \endcode
   };

   CharSourceRange Range;
   StringRef RawText;

   unsigned Kind : 8;
   unsigned StartColumn : 16;
   unsigned StartLine;
   unsigned EndLine;

   SingleRawComment(CharSourceRange Range, const SourceManager &SourceMgr);
   SingleRawComment(StringRef RawText, unsigned StartColumn);

   SingleRawComment(const SingleRawComment &) = default;
   SingleRawComment &operator=(const SingleRawComment &) = default;

   CommentKind getKind() const LLVM_READONLY {
      return static_cast<CommentKind>(Kind);
   }

   bool isOrdinary() const LLVM_READONLY {
      return getKind() == CommentKind::OrdinaryLine ||
            getKind() == CommentKind::OrdinaryBlock;
   }

   bool isLine() const LLVM_READONLY {
      return getKind() == CommentKind::OrdinaryLine ||
            getKind() == CommentKind::LineDoc;
   }
};

struct RawComment {
   ArrayRef<SingleRawComment> Comments;

   RawComment() {}
   RawComment(ArrayRef<SingleRawComment> Comments) : Comments(Comments) {}

   RawComment(const RawComment &) = default;
   RawComment &operator=(const RawComment &) = default;

   bool isEmpty() const {
      return Comments.empty();
   }

   CharSourceRange getCharSourceRange();
};

struct CommentInfo {
   StringRef Brief;
   RawComment Raw;
   uint32_t Group;
   uint32_t SourceOrder;
};

struct LineColumn {
   uint32_t Line = 0;
   uint32_t Column = 0;
   bool isValid() const { return Line && Column; }
};

struct BasicDeclLocs {
   StringRef SourceFilePath;
   LineColumn Loc;
   LineColumn StartLoc;
   LineColumn EndLoc;
};

} // namespace polar

#endif // POLARPHP_AST_RAW_COMMENT_H

