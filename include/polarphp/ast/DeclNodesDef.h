//===--- DeclNodes.def - Swift Declaration AST Metaprogramming --*- C++ -*-===//
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
// This file defines macros used for macro-metaprogramming with declarations.
//
//===----------------------------------------------------------------------===//



/// DECL(Id, Parent)
///   If the declaration node is not abstract, its enumerator value is
///   DeclKind::Id.  The node's class name is Id##Decl, and the name of
///   its base class (in the Decl hierarchy) is Parent.

/// An abstract declaration node is an abstract base class in the hierarchy;
/// it is never a most-derived type, and it does not have an enumerator in
/// DeclKind.
///
/// Most metaprograms do not care about abstract declarations, so the default
/// is to ignore them.
#ifndef ABSTRACT_DECL
#define ABSTRACT_DECL(Id, Parent)
#endif

/// CONTEXT_DECL(Id, Parent)
///   Used for Decls that are also DeclContexts. The default behavior is to do
///   the same as for Decl.
#ifndef CONTEXT_DECL
#define CONTEXT_DECL(Id, Parent) DECL(Id, Parent)
#endif

/// ITERABLE_NONGENERIC_DECL(Id, Parent)
///   Used for Decls that are also IterableDeclContexts and DeclContexts. The
///   default behavior is to do the same as for CONTEXT_DECL. This combination
///   does not exist at the moment.
#ifndef ITERABLE_NONGENERIC_DECL
#ifdef ITERABLE_DECL
#define ITERABLE_NONGENERIC_DECL(Id, Parent) ITERABLE_DECL(Id, Parent)
#else
#define ITERABLE_NONGENERIC_DECL(Id, Parent) CONTEXT_DECL(Id, Parent)
#endif
#endif

/// GENERIC_DECL(Id, Parent)
///   Used for Decls that are also GenericContexts. The default behavior is to
///   do the same as for CONTEXT_DECL.
#ifndef GENERIC_DECL
#define GENERIC_DECL(Id, Parent) CONTEXT_DECL(Id, Parent)
#endif

/// ITERABLE_GENERIC_DECL(Id, Parent)
///   Used for Decls that are also IterableDeclContexts and GenericContexts. The
///   default behavior is to do the same as for GENERIC_DECL.
#ifndef ITERABLE_GENERIC_DECL
#ifdef ITERABLE_DECL
#define ITERABLE_GENERIC_DECL(Id, Parent) ITERABLE_DECL(Id, Parent)
#else
#define ITERABLE_GENERIC_DECL(Id, Parent) GENERIC_DECL(Id, Parent)
#endif
#endif

/// CONTEXT_VALUE_DECL(Id, Parent)
///   Used for subclasses of ValueDecl that are also DeclContexts. The default
///   behavior is to do the same as for CONTEXT_DECL.
#ifndef CONTEXT_VALUE_DECL
#ifdef VALUE_DECL
#define CONTEXT_VALUE_DECL(Id, Parent) VALUE_DECL(Id, Parent)
#else
#define CONTEXT_VALUE_DECL(Id, Parent) CONTEXT_DECL(Id, Parent)
#endif
#endif

/// GENERIC_VALUE_DECL(Id, Parent)
///   Used for subclasses of ValueDecl that are also GenericContexts. The
///   default behavior is to do the same as for GENERIC_DECL.
#ifndef GENERIC_VALUE_DECL
#ifdef VALUE_DECL
#define GENERIC_VALUE_DECL(Id, Parent) VALUE_DECL(Id, Parent)
#else
#define GENERIC_VALUE_DECL(Id, Parent) GENERIC_DECL(Id, Parent)
#endif
#endif

/// ITERABLE_GENERIC_VALUE_DECL(Id, Parent)
///   Used for subclasses of ValueDecl that are also IterableDeclContexts and
///   GenericContexts. The default behavior is to do the same as for
///   ITERABLE_GENERIC_DECL.
#ifndef ITERABLE_GENERIC_VALUE_DECL
#ifdef VALUE_DECL
#define ITERABLE_GENERIC_VALUE_DECL(Id, Parent) VALUE_DECL(Id, Parent)
#else
#define ITERABLE_GENERIC_VALUE_DECL(Id, Parent) ITERABLE_GENERIC_DECL(Id,Parent)
#endif
#endif

/// OPERATOR_DECL(Id, Parent)
///   Used for subclasses of OperatorDecl. The default behavior is to do
///   the same as for Decl.
#ifndef OPERATOR_DECL
#define OPERATOR_DECL(Id, Parent) DECL(Id, Parent)
#endif

/// NOMINAL_TYPE_DECL(Id, Parent)
///   Used for subclasses of NominalTypeDecl.  The default behavior is
///   to do the same as for ITERABLE_GENERIC_VALUE_DECL.
#ifndef NOMINAL_TYPE_DECL
#define NOMINAL_TYPE_DECL(Id, Parent) ITERABLE_GENERIC_VALUE_DECL(Id, Parent)
#endif

/// ABSTRACT_FUNCTION_DECL(Id, Parent)
///   Used for subclasses of AbstractFunction. The default behavior is
///   to do the same as for GENERIC_VALUE_DECL.
#ifndef ABSTRACT_FUNCTION_DECL
#define ABSTRACT_FUNCTION_DECL(Id, Parent) GENERIC_VALUE_DECL(Id, Parent)
#endif

/// VALUE_DECL(Id, Parent)
///   Used for subclasses of ValueDecl.  The default behavior is to do
///   the same as for Decl.
#ifndef VALUE_DECL
#define VALUE_DECL(Id, Parent) DECL(Id, Parent)
#endif

/// A convenience for determining the range of declarations.  These will always
/// appear immediately after the last member.
#ifndef DECL_RANGE
#define DECL_RANGE(Id, First, Last)
#endif

#ifndef LAST_DECL
#define LAST_DECL(Id)
#endif

// NOTE: For performance, maximize the numer of [important] abstract Decls
// where "First_XYZDecl" == 0 to improve isa/dyn_cast code gen.

ABSTRACT_DECL(Value, Decl)
  ABSTRACT_DECL(Type, ValueDecl)
    ABSTRACT_DECL(GenericType, TypeDecl)
      ABSTRACT_DECL(NominalType, GenericTypeDecl)
        NOMINAL_TYPE_DECL(Enum, NominalTypeDecl)
        NOMINAL_TYPE_DECL(Struct, NominalTypeDecl)
        NOMINAL_TYPE_DECL(Class, NominalTypeDecl)
        NOMINAL_TYPE_DECL(Interface, NominalTypeDecl)
        DECL_RANGE(NominalType, Enum, Interface)
      GENERIC_VALUE_DECL(OpaqueType, GenericTypeDecl)
      GENERIC_VALUE_DECL(TypeAlias, GenericTypeDecl)
      DECL_RANGE(GenericType, Enum, TypeAlias)
    ABSTRACT_DECL(AbstractTypeParam, TypeDecl)
      VALUE_DECL(GenericTypeParam, AbstractTypeParamDecl)
      VALUE_DECL(AssociatedType, AbstractTypeParamDecl)
      DECL_RANGE(AbstractTypeParam, GenericTypeParam, AssociatedType)
    CONTEXT_VALUE_DECL(Module, TypeDecl)
    DECL_RANGE(Type, Enum, Module)
  ABSTRACT_DECL(AbstractStorage, ValueDecl)
    VALUE_DECL(Var, AbstractStorageDecl)
      VALUE_DECL(Param, VarDecl)
    GENERIC_VALUE_DECL(Subscript, AbstractStorageDecl)
    DECL_RANGE(AbstractStorage, Var, Subscript)
  ABSTRACT_DECL(AbstractFunction, ValueDecl)
    ABSTRACT_FUNCTION_DECL(Constructor, AbstractFunctionDecl)
    ABSTRACT_FUNCTION_DECL(Destructor, AbstractFunctionDecl)
    ABSTRACT_FUNCTION_DECL(Func, AbstractFunctionDecl)
      ABSTRACT_FUNCTION_DECL(Accessor, FuncDecl)
    DECL_RANGE(AbstractFunction, Constructor, Accessor)
  CONTEXT_VALUE_DECL(EnumElement, ValueDecl)
  DECL_RANGE(Value, Enum, EnumElement)

ITERABLE_GENERIC_DECL(Extension, Decl)
CONTEXT_DECL(TopLevelCode, Decl)
DECL(Import, Decl)
DECL(IfConfig, Decl)
DECL(PoundDiagnostic, Decl)
DECL(PrecedenceGroup, Decl)
DECL(MissingMember, Decl)
DECL(PatternBinding, Decl)
DECL(EnumCase, Decl)

ABSTRACT_DECL(Operator, Decl)
  OPERATOR_DECL(InfixOperator, OperatorDecl)
  OPERATOR_DECL(PrefixOperator, OperatorDecl)
  OPERATOR_DECL(PostfixOperator, OperatorDecl)
  DECL_RANGE(Operator, InfixOperator, PostfixOperator)

LAST_DECL(PostfixOperator)

#undef NOMINAL_TYPE_DECL
#undef CONTEXT_DECL
#undef ITERABLE_NONGENERIC_DECL
#undef GENERIC_DECL
#undef ITERABLE_GENERIC_DECL
#undef ITERABLE_DECL
#undef CONTEXT_VALUE_DECL
#undef GENERIC_VALUE_DECL
#undef ITERABLE_GENERIC_VALUE_DECL
#undef ABSTRACT_FUNCTION_DECL
#undef VALUE_DECL
#undef DECL_RANGE
#undef ABSTRACT_DECL
#undef OPERATOR_DECL
#undef DECL
#undef LAST_DECL
