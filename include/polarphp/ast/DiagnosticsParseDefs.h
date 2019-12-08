//===--- DiagnosticsParse.def - Diagnostics Text ----------------*- C++ -*-===//
//
// This source file is part of the Swift.org open source project
//
// Copyright (c) 2014 - 2018 Apple Inc. and the Swift project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See https://swift.org/LICENSE.txt for license information
// See https://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
//
//===----------------------------------------------------------------------===//
//
//  This file defines diagnostics emitted during lexing and parsing.
//  Each diagnostic is described using one of three kinds (error, warning, or
//  note) along with a unique identifier, category, options, and text, and is
//  followed by a signature describing the diagnostic argument kinds.
//
//===----------------------------------------------------------------------===//

#if !(defined(DIAG) || (defined(ERROR) && defined(WARNING) && defined(NOTE)))
#  error Must define either DIAG or the set {ERROR,WARNING,NOTE}
#endif

#ifndef ERROR
#  define ERROR(ID,Options,Text,Signature)   \
  DIAG(ERROR,ID,Options,Text,Signature)
#endif

#ifndef WARNING
#  define WARNING(ID,Options,Text,Signature) \
  DIAG(WARNING,ID,Options,Text,Signature)
#endif

#ifndef NOTE
#  define NOTE(ID,Options,Text,Signature) \
  DIAG(NOTE,ID,Options,Text,Signature)
#endif

#ifndef FIXIT
#  define FIXIT(ID, Text, Signature)
#endif

//==============================================================================
// MARK: Lexing and Parsing diagnostics
//==============================================================================

NOTE(opening_brace,none,
     "to match this opening '{'", ())
NOTE(opening_bracket,none,
     "to match this opening '['", ())
NOTE(opening_paren,none,
     "to match this opening '('", ())
NOTE(opening_angle,none,
     "to match this opening '<'", ())

ERROR(extra_rbrace,none,
      "extraneous '}' at top level", ())

ERROR(structure_overflow,Fatal,
      "structure nesting level exceeded maximum of %0", (unsigned))

ERROR(expected_close_to_if_directive,none,
      "expected #else or #endif at end of conditional compilation block", ())
ERROR(expected_close_after_else_directive,none,
      "further conditions after #else are unreachable", ())
ERROR(unexpected_conditional_compilation_block_terminator,none,
      "unexpected conditional compilation block terminator", ())
ERROR(incomplete_conditional_compilation_directive,none,
      "incomplete condition in conditional compilation directive", ())
ERROR(extra_tokens_conditional_compilation_directive,none,
      "extra tokens following conditional compilation directive", ())
ERROR(unexpected_rbrace_in_conditional_compilation_block,none,
      "unexpected '}' in conditional compilation block", ())
ERROR(unexpected_if_following_else_compilation_directive,none,
      "unexpected 'if' keyword following '#else' conditional compilation "
      "directive; did you mean '#elseif'?",
      ())

ERROR(pound_diagnostic_expected_string,none,
      "expected string literal in %select{#warning|#error}0 directive",(bool))
ERROR(pound_diagnostic_expected,none,
      "expected '%0' in %select{#warning|#error}1 directive",(StringRef,bool))
ERROR(pound_diagnostic_expected_parens,none,
      "%select{#warning|#error}0 directive requires parentheses",(bool))
ERROR(pound_diagnostic_interpolation,none,
      "string interpolation is not allowed in %select{#warning|#error}0 directives",(bool))
ERROR(extra_tokens_pound_diagnostic_directive,none,
      "extra tokens following %select{#warning|#error}0 directive", (bool))

ERROR(sourceLocation_expected,none,
      "expected '%0' in #sourceLocation directive", (StringRef))

ERROR(unexpected_line_directive,none,
      "parameterless closing #sourceLocation() directive "
      "without prior opening #sourceLocation(file:,line:) directive", ())
ERROR(expected_line_directive_number,none,
      "expected starting line number for #sourceLocation directive", ())
ERROR(expected_line_directive_name,none,
      "expected filename string literal for #sourceLocation directive", ())
ERROR(extra_tokens_line_directive,none,
      "extra tokens at the end of #sourceLocation directive", ())
ERROR(line_directive_line_zero,none,
      "the line number needs to be greater than zero", ())

WARNING(escaped_parameter_name,none,
        "keyword '%0' does not need to be escaped in argument list",
        (StringRef))

ERROR(forbidden_interpolated_string,none,
      "%0 cannot be an interpolated string literal", (StringRef))
ERROR(forbidden_extended_escaping_string,none,
      "%0 cannot be an extended escaping string literal", (StringRef))

//------------------------------------------------------------------------------
// MARK: Lexer diagnostics
//------------------------------------------------------------------------------

WARNING(lex_nul_character,none,
        "nul character embedded in middle of file", ())
ERROR(lex_utf16_bom_marker,none,
      "input files must be encoded as UTF-8 instead of UTF-16", ())

ERROR(lex_hashbang_not_allowed,none,
      "hashbang line is allowed only in the main file", ())

ERROR(lex_unprintable_ascii_character,none,
      "unprintable ASCII character found in source file", ())
ERROR(lex_invalid_utf8,none,
      "invalid UTF-8 found in source file", ())
ERROR(lex_single_quote_string,none,
      "single-quoted string literal found, use '\"'", ())
ERROR(lex_invalid_curly_quote,none,
      "unicode curly quote found, replace with '\"'", ())
NOTE(lex_confusable_character,none,
      "unicode character '%0' looks similar to '%1'; did you mean to use '%1'?",
      (StringRef, StringRef))
WARNING(lex_nonbreaking_space,none,
        "non-breaking space (U+00A0) used instead of regular space", ())

ERROR(lex_unterminated_block_comment,none,
      "unterminated '/*' comment", ())
NOTE(lex_comment_start,none,
     "comment started here", ())


ERROR(lex_unterminated_string,none,
      "unterminated string literal", ())
ERROR(lex_invalid_escape,none,
       "invalid escape sequence in literal", ())
ERROR(lex_invalid_u_escape,none,
      "\\u{...} escape sequence expects between 1 and 8 hex digits", ())
ERROR(lex_invalid_u_escape_rbrace,none,
      "expected '}' in \\u{...} escape sequence", ())
ERROR(lex_invalid_escape_delimiter,none,
      "too many '#' characters in delimited escape", ())
ERROR(lex_invalid_closing_delimiter,none,
      "too many '#' characters in closing delimiter", ())

ERROR(lex_invalid_unicode_scalar,none,
      "invalid unicode scalar", ())
ERROR(lex_unicode_escape_braces,none,
      "expected hexadecimal code in braces after unicode escape", ())
ERROR(lex_illegal_multiline_string_start,none,
      "multi-line string literal content must begin on a new line", ())
ERROR(lex_illegal_multiline_string_end,none,
      "multi-line string literal closing delimiter must begin on a new line", ())
ERROR(lex_multiline_string_indent_inconsistent,none,
      "%select{unexpected space in|unexpected tab in|insufficient}2 indentation of "
      "%select{line|next %1 lines}0 in multi-line string literal",
      (bool, unsigned, unsigned))
NOTE(lex_multiline_string_indent_should_match_here,none,
      "should match %select{space|tab}0 here", (unsigned))
NOTE(lex_multiline_string_indent_change_line,none,
      "change indentation of %select{this line|these lines}0 to match closing delimiter", (bool))
ERROR(lex_escaped_newline_at_lastline,none,
      "escaped newline at the last line is not allowed", ())

ERROR(lex_invalid_character,none,
       "invalid character in source file", ())
ERROR(lex_invalid_identifier_start_character,none,
       "an identifier cannot begin with this character", ())
ERROR(lex_expected_digit_in_fp_exponent,none,
       "expected a digit in floating point exponent", ())
ERROR(lex_invalid_digit_in_fp_exponent,none,
      "'%0' is not a valid %select{digit|first character}1 in floating point exponent",
      (StringRef, bool))
ERROR(lex_invalid_digit_in_int_literal,none,
      "'%0' is not a valid %select{binary digit (0 or 1)|octal digit (0-7)|"
      "digit|hexadecimal digit (0-9, A-F)}1 in integer literal",
      (StringRef, unsigned))
ERROR(lex_expected_binary_exponent_in_hex_float_literal,none,
      "hexadecimal floating point literal must end with an exponent", ())
ERROR(lex_unexpected_block_comment_end,none,
      "unexpected end of block comment", ())
ERROR(lex_unary_equal,none,
      "'=' must have consistent whitespace on both sides", ())
ERROR(extra_whitespace_period,none,
      "extraneous whitespace after '.' is not permitted", ())
ERROR(lex_editor_placeholder,none,
      "editor placeholder in source file", ())
WARNING(lex_editor_placeholder_in_playground,none,
        "editor placeholder in source file", ())
ERROR(lex_conflict_marker_in_file,none,
      "source control conflict marker in source file", ())
//------------------------------------------------------------------------------
// MARK: Declaration parsing diagnostics
//------------------------------------------------------------------------------

NOTE(note_in_decl_extension,none,
     "in %select{declaration|extension}0 of %1", (bool, Identifier))
ERROR(line_directive_style_deprecated,none,
        "#line directive was renamed to #sourceLocation",
        ())

ERROR(declaration_same_line_without_semi,none,
      "consecutive declarations on a line must be separated by ';'", ())

ERROR(expected_decl,none,
      "expected declaration", ())
ERROR(expected_identifier_in_decl,none,
      "expected identifier in %0 declaration", (StringRef))
ERROR(expected_keyword_in_decl,none,
      "expected '%0' keyword in %1 declaration", (StringRef, DescriptiveDeclKind))
ERROR(number_cant_start_decl_name,none,
      "%0 name can only start with a letter or underscore, not a number",
      (StringRef))
ERROR(expected_identifier_after_case_comma, PointsToFirstBadToken,
      "expected identifier after comma in enum 'case' declaration", ())
ERROR(decl_redefinition,none,
      "definition conflicts with previous value", ())
ERROR(let_cannot_be_computed_property,none,
      "'let' declarations cannot be computed properties", ())
ERROR(let_cannot_be_observing_property,none,
      "'let' declarations cannot be observing properties", ())
ERROR(let_cannot_be_addressed_property,none,
      "'let' declarations cannot have addressors", ())
ERROR(disallowed_var_multiple_getset,none,
      "'var' declarations with multiple variables cannot have explicit"
      " getters/setters", ())

ERROR(disallowed_init,none,
      "initial value is not allowed here", ())
ERROR(var_init_self_referential,none,
      "variable used within its own initial value", ())

ERROR(disallowed_enum_element,none,
      "enum 'case' is not allowed outside of an enum", ())
ERROR(decl_inner_scope,none,
      "declaration is only valid at file scope", ())

ERROR(decl_not_static,none,
      "declaration cannot be marked %0", (StaticSpellingKind))

ERROR(cskeyword_not_attribute,none,
      "'%0' is a declaration modifier, not an attribute", (StringRef))

ERROR(decl_already_static,none,
      "%0 cannot appear after another 'static' or 'class' keyword", (StaticSpellingKind))

ERROR(enum_case_dot_prefix,none,
      "extraneous '.' in enum 'case' declaration", ())

// Variable getters/setters
ERROR(static_var_decl_global_scope,none,
      "%select{%error|static properties|class properties}0 may only be declared on a type",
      (StaticSpellingKind))
ERROR(computed_property_no_accessors, none,
      "%select{computed property|subscript}0 must have accessors specified", (bool))
ERROR(expected_getset_in_protocol,none,
      "expected get or set in a protocol property", ())
ERROR(computed_property_missing_type,none,
      "computed property must have an explicit type", ())
ERROR(getset_nontrivial_pattern,none,
      "getter/setter can only be defined for a single variable", ())
ERROR(expected_rbrace_in_getset,none,
      "expected '}' at end of variable get/set clause", ())
ERROR(duplicate_accessor,none,
      "%select{variable|subscript}0 already has %1", (unsigned, StringRef))
ERROR(conflicting_accessor,none,
      "%select{variable|subscript}0 cannot provide both %1 and %2",
      (unsigned, StringRef, StringRef))
NOTE(previous_accessor,none,
     "%select{|previous definition of }1%0 %select{defined |}1here", (StringRef, bool))
ERROR(expected_accessor_parameter_name,none,
      "expected %select{setter|willSet|didSet}0 parameter name",
      (unsigned))
ERROR(expected_rparen_set_name,none,
      "expected ')' after setter parameter name",())
ERROR(expected_rparen_willSet_name,none,
      "expected ')' after willSet parameter name",())
ERROR(expected_rparen_didSet_name,none,
      "expected ')' after didSet parameter name",())
ERROR(expected_lbrace_accessor,PointsToFirstBadToken,
      "expected '{' to start %0 definition", (StringRef))
ERROR(expected_accessor_kw,none,
      "expected 'get', 'set', 'willSet', or 'didSet' keyword to "
      "start an accessor definition",())
ERROR(missing_getter,none,
      "%select{variable|subscript}0 with %1 must also have a getter",
      (unsigned, StringRef))
ERROR(missing_reading_accessor,none,
      "%select{variable|subscript}0 with %1 must also have "
      "a getter, addressor, or 'read' accessor",
      (unsigned, StringRef))
ERROR(observing_accessor_conflicts_with_accessor,none,
      "%select{'willSet'|'didSet'}0 cannot be provided together with %1",
      (unsigned, StringRef))
ERROR(observing_accessor_in_subscript,none,
      "%select{'willSet'|'didSet'}0 is not allowed in subscripts", (unsigned))
ERROR(getset_cannot_be_implied,none,
      "variable with implied type cannot have implied getter/setter", ())

// Import
ERROR(decl_expected_module_name,none,
      "expected module name in import declaration", ())

// Extension
ERROR(expected_lbrace_extension,PointsToFirstBadToken,
      "expected '{' in extension", ())
ERROR(expected_rbrace_extension,none,
      "expected '}' at end of extension", ())
ERROR(extension_type_expected,none,
      "expected type name in extension declaration", ())

// TypeAlias
ERROR(expected_equal_in_typealias,PointsToFirstBadToken,
      "expected '=' in type alias declaration", ())
ERROR(expected_type_in_typealias,PointsToFirstBadToken,
      "expected type in type alias declaration", ())
ERROR(expected_type_in_associatedtype,PointsToFirstBadToken,
      "expected type in associated type declaration", ())
ERROR(associated_type_generic_parameter_list,PointsToFirstBadToken,
      "associated types must not have a generic parameter list", ())


// Func
ERROR(func_decl_without_paren,PointsToFirstBadToken,
      "expected '(' in argument list of function declaration", ())
ERROR(static_func_decl_global_scope,none,
      "%select{%error|static methods|class methods}0 may only be declared on a type",
      (StaticSpellingKind))
ERROR(func_decl_expected_arrow,none,
      "expected '->' after function parameter tuple", ())
ERROR(operator_static_in_protocol,none,
      "operator '%0' declared in protocol must be 'static'",
      (StringRef))

// Enum
ERROR(expected_lbrace_enum,PointsToFirstBadToken,
      "expected '{' in enum", ())
ERROR(expected_rbrace_enum,none,
      "expected '}' at end of enum", ())

// Struct
ERROR(expected_lbrace_struct,PointsToFirstBadToken,
      "expected '{' in struct", ())
ERROR(expected_rbrace_struct,none,
      "expected '}' in struct", ())

// Class
ERROR(expected_lbrace_class,PointsToFirstBadToken,
      "expected '{' in class", ())
ERROR(expected_rbrace_class,none,
      "expected '}' in class", ())
ERROR(expected_colon_class, PointsToFirstBadToken,
      "expected ':' to begin inheritance clause",())

// Protocol
ERROR(generic_arguments_protocol,PointsToFirstBadToken,
      "protocols do not allow generic parameters; use associated types instead",
      ())
ERROR(expected_lbrace_protocol,PointsToFirstBadToken,
      "expected '{' in protocol type", ())
ERROR(expected_rbrace_protocol,none,
      "expected '}' in protocol", ())
ERROR(protocol_setter_name,none,
      "setter in a protocol cannot have a name", ())
ERROR(protocol_method_with_body,none,
      "protocol methods must not have bodies", ())
ERROR(protocol_init_with_body,none,
      "protocol initializers must not have bodies", ())

// Subscripting
ERROR(subscript_decl_wrong_scope,none,
      "'subscript' functions may only be declared within a type", ())
ERROR(expected_lparen_subscript,PointsToFirstBadToken,
      "expected '(' for subscript parameters", ())
ERROR(subscript_has_name,PointsToFirstBadToken,
      "subscripts cannot have a name", ())
ERROR(expected_arrow_subscript,PointsToFirstBadToken,
      "expected '->' for subscript element type", ())
ERROR(expected_type_subscript,PointsToFirstBadToken,
      "expected subscripting element type", ())
ERROR(expected_lbrace_subscript,PointsToFirstBadToken,
      "expected '{' in subscript to specify getter and setter implementation",
      ())
ERROR(expected_lbrace_subscript_protocol,PointsToFirstBadToken,
      "subscript in protocol must have explicit { get } or "
      "{ get set } specifier", ())
ERROR(subscript_without_get,none,
      "subscript declarations must have a getter", ())

// initializer
ERROR(invalid_nested_init,none,
      "missing '%select{super.|self.}0' at initializer invocation", (bool))
ERROR(initializer_decl_wrong_scope,none,
      "initializers may only be declared within a type", ())
ERROR(expected_lparen_initializer,PointsToFirstBadToken,
      "expected '(' for initializer parameters", ())
ERROR(initializer_has_name,PointsToFirstBadToken,
      "initializers cannot have a name", ())

// Destructor
ERROR(destructor_decl_outside_class,none,
      "deinitializers may only be declared within a class", ())
ERROR(expected_lbrace_destructor,PointsToFirstBadToken,
      "expected '{' for deinitializer", ())
ERROR(destructor_has_name,PointsToFirstBadToken,
      "deinitializers cannot have a name", ())

ERROR(opened_destructor_expected_rparen,none,
      "expected ')' to close parameter list", ())
ERROR(destructor_params,none,
      "no parameter clause allowed on deinitializer", ())

// Operator
ERROR(operator_decl_inner_scope,none,
      "'operator' may only be declared at file scope", ())
ERROR(expected_operator_name_after_operator,PointsToFirstBadToken,
      "expected operator name in operator declaration", ())
ERROR(identifier_when_expecting_operator,PointsToFirstBadToken,
      "%0 is considered to be an identifier, not an operator", (Identifier))

ERROR(deprecated_operator_body,PointsToFirstBadToken,
      "operator should no longer be declared with body", ())
ERROR(deprecated_operator_body_use_group,PointsToFirstBadToken,
      "operator should no longer be declared with body; "
      "use a precedence group instead", ())
ERROR(operator_decl_no_fixity,none,
      "operator must be declared as 'prefix', 'postfix', or 'infix'", ())

ERROR(operator_decl_expected_type,none,
      "expected designated type in operator declaration", ())
ERROR(operator_decl_trailing_comma,none,
      "trailing comma in operator declaration", ())

// PrecedenceGroup
ERROR(precedencegroup_not_infix,none,
      "only infix operators may declare a precedence", ())
ERROR(expected_precedencegroup_name,none,
      "expected identifier after 'precedencegroup'", ())
ERROR(expected_precedencegroup_lbrace,none,
      "expected '{' after name of precedence group", ())

ERROR(expected_precedencegroup_attribute,none,
      "expected operator attribute identifier in precedence group body", ())
ERROR(unknown_precedencegroup_attribute,none,
      "'%0' is not a valid precedence group attribute", (StringRef))
ERROR(expected_precedencegroup_attribute_colon,none,
      "expected colon after attribute name in precedence group", (StringRef))
ERROR(precedencegroup_attribute_redeclared,none,
      "'%0' attribute for precedence group declared multiple times",
      (StringRef))
ERROR(expected_precedencegroup_associativity,none,
      "expected 'none', 'left', or 'right' after 'associativity'", ())
ERROR(expected_precedencegroup_assignment,none,
      "expected 'true' or 'false' after 'assignment'", ())
ERROR(expected_precedencegroup_relation,none,
      "expected name of related precedence group after '%0'",
      (StringRef))

// SIL
ERROR(inout_not_attribute, none,
      "@inout is no longer an attribute", ())
ERROR(only_allowed_in_sil,none,
      "'%0' only allowed in SIL modules", (StringRef))
ERROR(expected_sil_type,none,
      "expected type in SIL code", ())
ERROR(expected_sil_colon_value_ref,none,
      "expected ':' before type in SIL value reference", ())
ERROR(expected_sil_value_name,none,
      "expected SIL value name", ())
ERROR(expected_sil_type_kind,none,
      "expected SIL type to %0", (StringRef))
ERROR(expected_sil_constant,none,
      "expected constant in SIL code", ())
ERROR(referenced_value_no_accessor,none,
      "referenced declaration has no %select{getter|setter}0", (unsigned))
ERROR(expected_sil_value_ownership_kind,none,
      "expected value ownership kind in SIL code", ())
ERROR(silfunc_and_silarg_have_incompatible_sil_value_ownership,none,
      "SILFunction and SILArgument have mismatching ValueOwnershipKinds. "
      "Function type specifies: '@%0'. SIL argument specifies: '@%1'.",
      (StringRef, StringRef))
ERROR(expected_sil_colon,none,
      "expected ':' before %0", (StringRef))
ERROR(expected_sil_tuple_index,none,
      "expected tuple element index", ())

// SIL Values
ERROR(sil_value_redefinition,none,
      "redefinition of value '%0'", (StringRef))
ERROR(sil_value_use_type_mismatch,none,
      "value '%0' defined with mismatching type %1 (expected %2)", (StringRef, Type, Type))
ERROR(sil_value_def_type_mismatch,none,
      "value '%0' used with mismatching type %1 (expected %2)", (StringRef, Type, Type))
ERROR(sil_use_of_undefined_value,none,
      "use of undefined value '%0'", (StringRef))
NOTE(sil_prior_reference,none,
     "prior reference was here", ())

// SIL Locations
ERROR(expected_colon_in_sil_location,none,
      "expected ':' in SIL location", ())
ERROR(sil_invalid_line_in_sil_location,none,
      "line number must be a positive integer", ())
ERROR(sil_invalid_column_in_sil_location,none,
      "column number must be a positive integer", ())
ERROR(sil_invalid_scope_slot,none,
      "scope number must be a positive integer ", ())
ERROR(sil_scope_undeclared,none,
      "scope number %0 needs to be declared before first use", (unsigned))
ERROR(sil_scope_redefined,none,
      "scope number %0 is already defined", (unsigned))

// SIL Instructions
ERROR(expected_sil_instr_start_of_line,none,
      "SIL instructions must be at the start of a line", ())
ERROR(expected_equal_in_sil_instr,none,
      "expected '=' in SIL instruction", ())
ERROR(wrong_result_count_in_sil_instr,none,
      "wrong number of results for SIL instruction, expected %0", (unsigned))
ERROR(expected_sil_instr_opcode,none,
      "expected SIL instruction opcode", ())
ERROR(expected_tok_in_sil_instr,none,
      "expected '%0' in SIL instruction", (StringRef))
ERROR(sil_property_generic_signature_mismatch,none,
      "sil_property generic signature must match original declaration", ())
ERROR(sil_string_no_encoding,none,
      "string_literal instruction requires an encoding", ())
ERROR(sil_string_invalid_encoding,none,
      "unknown string literal encoding '%0'", (StringRef))
ERROR(expected_tuple_type_in_tuple,none,
      "tuple instruction requires a tuple type", ())
ERROR(sil_tuple_inst_wrong_value_count,none,
      "tuple instruction requires %0 values", (unsigned))
ERROR(sil_tuple_inst_wrong_field,none,
      "tuple instruction requires a field number", ())
ERROR(sil_struct_inst_wrong_field,none,
      "struct instruction requires a field name", ())
ERROR(sil_ref_inst_wrong_field,none,
      "ref_element_addr instruction requires a field name", ())
ERROR(sil_invalid_instr_operands,none,
      "invalid instruction operands", ())
ERROR(sil_operand_not_address,none,
      "%0 operand of '%1' must have address type", (StringRef, StringRef))
ERROR(sil_operand_not_ref_storage_address,none,
      "%0 operand of '%1' must have address of %2 type",
      (StringRef, StringRef, ReferenceOwnership))
ERROR(sil_integer_literal_not_integer_type,none,
      "integer_literal instruction requires a 'Builtin.Int<n>' type", ())
ERROR(sil_integer_literal_not_well_formed,none,
      "integer_literal value not well-formed for type %0", (Type))
ERROR(sil_float_literal_not_float_type,none,
      "float_literal instruction requires a 'Builtin.FP<n>' type", ())
ERROR(sil_substitutions_on_non_polymorphic_type,none,
      "apply of non-polymorphic function cannot have substitutions", ())
ERROR(sil_witness_method_not_protocol,none,
      "witness_method is not a protocol method", ())
ERROR(sil_witness_method_type_does_not_conform,none,
      "witness_method type does not conform to protocol", ())
ERROR(sil_member_decl_not_found,none, "member not found", ())
ERROR(sil_named_member_decl_not_found,none,
      "member %0 not found in type %1", (DeclName, Type))
ERROR(sil_member_lookup_bad_type,none,
      "cannot lookup member %0 in non-nominal, non-module type %1",
      (DeclName, Type))
ERROR(sil_member_decl_type_mismatch,none,
      "member defined with mismatching type %0 (expected %1)", (Type, Type))
ERROR(sil_substitution_mismatch,none,
      "substitution replacement type %0 does not conform to protocol %1",
      (Type, Type))
ERROR(sil_not_class,none,
      "substitution replacement type %0 is not a class type",
      (Type))
ERROR(sil_missing_substitutions,none,
      "missing substitutions", ())
ERROR(sil_too_many_substitutions,none,
      "too many substitutions", ())
ERROR(sil_dbg_unknown_key,none,
      "unknown key '%0' in debug variable declaration", (StringRef))
ERROR(sil_objc_with_tail_elements,none,
      "alloc_ref [objc] cannot have tail allocated elements", ())
ERROR(sil_expected_access_kind,none,
      "%0 instruction must have explicit access kind", (StringRef))
ERROR(sil_expected_access_enforcement,none,
      "%0 instruction must have explicit access enforcement", (StringRef))

ERROR(sil_keypath_expected_component_kind,none,
      "expected keypath component kind", ())
ERROR(sil_keypath_unknown_component_kind,none,
      "unknown keypath component kind %0", (Identifier))
ERROR(sil_keypath_computed_property_missing_part,none,
      "keypath %select{gettable|settable}0_property component needs an "
      "%select{id and getter|id, getter, and setter}0", (bool))
ERROR(sil_keypath_no_root,none,
      "keypath must have a root component declared",())
ERROR(sil_keypath_index_not_hashable,none,
      "key path index type %0 does not conform to Hashable", (Type))
ERROR(sil_keypath_index_operand_type_conflict,none,
      "conflicting types for key path operand %0: %1 vs. %2",
      (unsigned, Type, Type))
ERROR(sil_keypath_no_use_of_operand_in_pattern,none,
      "operand %0 is not referenced by any component in the pattern",
      (unsigned))

// SIL Basic Blocks
ERROR(expected_sil_block_name,none,
      "expected basic block name or '}'", ())
ERROR(expected_sil_block_colon,none,
      "expected ':' after basic block name", ())
ERROR(sil_undefined_basicblock_use,none,
      "use of undefined basic block %0", (Identifier))
ERROR(sil_basicblock_redefinition,none,
      "redefinition of basic block %0", (Identifier))
ERROR(sil_basicblock_arg_rparen,none,
      "expected ')' in basic block argument list", ())

// SIL Functions
ERROR(expected_sil_function_name,none,
      "expected SIL function name", ())
ERROR(expected_sil_rbrace,none,
      "expected '}' at the end of a sil body", ())
ERROR(expected_sil_function_type, none,
      "sil function expected to have SIL function type", ())
ERROR(sil_dynamically_replaced_func_not_found,none,
      "dynamically replaced function not found %0", (Identifier))
ERROR(sil_availability_expected_version,none,
      "expected version number in 'available' attribute", ())

// SIL Stage
ERROR(expected_sil_stage_name, none,
      "expected 'raw' or 'canonical' after 'sil_stage'", ())
ERROR(multiple_sil_stage_decls, none,
      "sil_stage declared multiple times", ())

// SIL VTable
ERROR(expected_sil_vtable_colon,none,
      "expected ':' in a vtable entry", ())
ERROR(sil_vtable_func_not_found,none,
      "sil function not found %0", (Identifier))
ERROR(sil_vtable_class_not_found,none,
      "sil class not found %0", (Identifier))
ERROR(sil_vtable_bad_entry_kind,none,
      "expected 'inherited' or 'override'", ())
ERROR(sil_vtable_expect_rsquare,none,
      "expected ']' after vtable entry kind", ())

// SIL Global
ERROR(sil_global_variable_not_found,none,
      "sil global not found %0", (Identifier))

// SIL Witness Table
ERROR(expected_sil_witness_colon,none,
      "expected ':' in a witness table", ())
ERROR(expected_sil_witness_lparen,none,
      "expected '(' in a witness table", ())
ERROR(expected_sil_witness_rparen,none,
      "expected ')' in a witness table", ())
ERROR(sil_witness_func_not_found,none,
      "sil function not found %0", (Identifier))
ERROR(sil_witness_protocol_not_found,none,
      "sil protocol not found %0", (Identifier))
ERROR(sil_witness_assoc_not_found,none,
      "sil associated type decl not found %0", (Identifier))
ERROR(sil_witness_assoc_conf_not_found,none,
      "sil associated type path for conformance not found %0", (StringRef))
ERROR(sil_witness_protocol_conformance_not_found,none,
      "sil protocol conformance not found", ())

// SIL Coverage Map
ERROR(sil_coverage_invalid_hash, none,
      "expected coverage hash", ())
ERROR(sil_coverage_expected_lbrace, none,
      "expected '{' in coverage map", ())
ERROR(sil_coverage_expected_loc, none,
      "expected line:column pair", ())
ERROR(sil_coverage_expected_arrow, none,
      "expected '->' after start location", ())
ERROR(sil_coverage_expected_colon, none,
      "expected ':' after source range", ())
ERROR(sil_coverage_invalid_counter, none,
      "expected counter expression, id, or 'zero'", ())
ERROR(sil_coverage_expected_rparen, none,
      "expected ')' to end counter expression", ())
ERROR(sil_coverage_expected_quote, none,
      "expected quotes surrounding PGO function name", ())
ERROR(sil_coverage_invalid_operator, none,
      "expected '+' or '-'", ())

//------------------------------------------------------------------------------
// MARK: Type parsing diagnostics
//------------------------------------------------------------------------------

ERROR(expected_type,PointsToFirstBadToken,
      "expected type", ())
ERROR(expected_init_value,PointsToFirstBadToken,
      "expected initial value after '='", ())

// Named types
ERROR(expected_identifier_in_dotted_type,PointsToFirstBadToken,
      "expected identifier in dotted type", ())
ERROR(expected_identifier_for_type,PointsToFirstBadToken,
      "expected identifier for type name", ())
ERROR(expected_rangle_generic_arg_list,PointsToFirstBadToken,
      "expected '>' to complete generic argument list", ())


// Function types
ERROR(expected_type_function_result,PointsToFirstBadToken,
      "expected type for function result", ())
ERROR(generic_non_function,PointsToFirstBadToken,
      "only syntactic function types can be generic", ())
ERROR(rethrowing_function_type,none,
      "only function declarations may be marked 'rethrows'; "
      "did you mean 'throws'?", ())
ERROR(throws_in_wrong_position,none,
      "'throws' may only occur before '->'", ())
ERROR(rethrows_in_wrong_position,none,
      "'rethrows' may only occur before '->'", ())
ERROR(throw_in_function_type,none,
      "expected throwing specifier; did you mean 'throws'?", ())
ERROR(expected_type_before_arrow,none,
      "expected type before '->'", ())
ERROR(expected_type_after_arrow,none,
      "expected type after '->'", ())
ERROR(function_type_argument_label,none,
      "function types cannot have argument labels; use '_' before %0",
      (Identifier))
ERROR(expected_dynamic_func_attr,none,
      "expected a dynamically_replaceable function", ())

// Enum Types
ERROR(expected_expr_enum_case_raw_value,PointsToFirstBadToken,
      "expected expression after '=' in 'case'", ())
ERROR(nonliteral_enum_case_raw_value,PointsToFirstBadToken,
      "raw value for enum case must be a literal", ())

// Collection Types
ERROR(new_array_syntax,none,
      "array types are now written with the brackets around the element type",
      ())
ERROR(expected_rbracket_array_type,PointsToFirstBadToken,
      "expected ']' in array type", ())
ERROR(expected_element_type,PointsToFirstBadToken,
      "expected element type", ())
ERROR(expected_dictionary_value_type,PointsToFirstBadToken,
      "expected dictionary value type", ())
ERROR(expected_rbracket_dictionary_type,PointsToFirstBadToken,
      "expected ']' in dictionary type", ())
ERROR(extra_rbracket,PointsToFirstBadToken,
      "unexpected ']' in type; did you mean to write an array type?", ())
ERROR(extra_colon,PointsToFirstBadToken,
      "unexpected ':' in type; did you mean to write a dictionary type?", ())
WARNING(subscript_array_element, none,
        "unexpected subscript in array literal; did you mean to write two "
        "separate elements instead?", ())
NOTE(subscript_array_element_fix_it_add_comma, none, "add a separator between "
     "the elements", ())
NOTE(subscript_array_element_fix_it_remove_space, none,
     "remove the space between the elements to silence this warning", ())

// Tuple Types
ERROR(expected_rparen_tuple_type_list,none,
      "expected ')' at end of tuple list", ())
ERROR(multiple_ellipsis_in_tuple,none,
      "only a single element can be variadic", ())
ERROR(tuple_type_init,none,
      "default argument not permitted in a tuple type", ())
ERROR(protocol_method_argument_init,none,
      "default argument not permitted in a protocol method", ())
ERROR(protocol_init_argument_init,none,
      "default argument not permitted in a protocol initializer", ())
ERROR(tuple_type_multiple_labels,none,
      "tuple element cannot have two labels", ())

// Protocol Types
ERROR(expected_rangle_protocol,PointsToFirstBadToken,
      "expected '>' to complete protocol-constrained type", ())

ERROR(deprecated_protocol_composition,none,
      "'protocol<...>' composition syntax has been removed; join the protocols using '&'", ())
ERROR(deprecated_protocol_composition_single,none,
      "'protocol<...>' composition syntax has been removed and is not needed here", ())
ERROR(deprecated_any_composition,none,
      "'protocol<>' syntax has been removed; use 'Any' instead", ())

// SIL box Types
ERROR(sil_box_expected_var_or_let,none,
      "expected 'var' or 'let' to introduce SIL box field type", ())
ERROR(sil_box_expected_r_brace,none,
      "expected '}' to complete SIL box field type list", ())
ERROR(sil_box_expected_r_angle,none,
      "expected '>' to complete SIL box generic argument list", ())

// Opaque types
ERROR(opaque_mid_composition,none,
      "'some' should appear at the beginning of a composition", ())

//------------------------------------------------------------------------------
// MARK: Layout constraint diagnostics
//------------------------------------------------------------------------------

ERROR(layout_size_should_be_positive,none,
      "expected non-negative size to be specified in layout constraint", ())
ERROR(layout_alignment_should_be_positive,none,
      "expected non-negative alignment to be specified in layout constraint", ())
ERROR(expected_rparen_layout_constraint,none,
      "expected ')' to complete layout constraint", ())
ERROR(layout_constraints_only_inside_specialize_attr,none,
      "layout constraints are only allowed inside '_specialize' attributes", ())

//------------------------------------------------------------------------------
// MARK: Pattern parsing diagnostics
//------------------------------------------------------------------------------

ERROR(expected_pattern,PointsToFirstBadToken,
      "expected pattern", ())
ERROR(keyword_cant_be_identifier,none,
      "keyword '%0' cannot be used as an identifier here", (StringRef))
ERROR(repeated_identifier,none,
      "found an unexpected second identifier in %0 declaration; is there an accidental break?", (StringRef))
NOTE(join_identifiers,none,
     "join the identifiers together", ())
NOTE(join_identifiers_camel_case,none,
     "join the identifiers together with camel-case", ())
NOTE(backticks_to_escape,none,
     "if this name is unavoidable, use backticks to escape it", ())
ERROR(expected_rparen_tuple_pattern_list,none,
      "expected ')' at end of tuple pattern", ())
ERROR(untyped_pattern_ellipsis,none,
      "'...' cannot be applied to a subpattern which is not explicitly typed", ())
ERROR(no_default_arg_closure,none,
      "default arguments are not allowed in closures", ())
ERROR(no_default_arg_curried,none,
      "default arguments are not allowed in curried parameter lists", ())
ERROR(var_pattern_in_var,none,
      "'%select{var|let}0' cannot appear nested inside another 'var' or "
      "'let' pattern", (unsigned))
ERROR(extra_var_in_multiple_pattern_list,none,
      "%0 must be bound in every pattern", (Identifier))
ERROR(let_pattern_in_immutable_context,none,
      "'let' pattern cannot appear nested in an already immutable context", ())
ERROR(specifier_must_have_type,none,
      "%0 arguments must have a type specified", (StringRef))
ERROR(expected_rparen_parameter,PointsToFirstBadToken,
      "expected ')' in parameter", ())
ERROR(expected_parameter_type,PointsToFirstBadToken,
      "expected parameter type following ':'", ())
ERROR(expected_parameter_name,PointsToFirstBadToken,
      "expected parameter name followed by ':'", ())
ERROR(expected_parameter_colon,PointsToFirstBadToken,
      "expected ':' following argument label and parameter name", ())
ERROR(expected_assignment_instead_of_comparison_operator,none,
      "expected '=' instead of '==' to assign default value for parameter", ())
ERROR(multiple_parameter_ellipsis,none,
      "only a single variadic parameter '...' is permitted", ())
ERROR(parameter_vararg_default,none,
      "variadic parameter cannot have a default value", ())
ERROR(parameter_specifier_as_attr_disallowed,none,
      "'%0' before a parameter name is not allowed, place it before the parameter type instead",
      (StringRef))
ERROR(parameter_specifier_repeated,none,
      "parameter must not have multiple '__owned', 'inout', or '__shared' specifiers", ())
WARNING(parameter_let_var_as_attr,none,
      "'%0' in this position is interpreted as an argument label",
      (StringRef))


WARNING(parameter_extraneous_double_up,none,
        "extraneous duplicate parameter name; %0 already has an argument "
        "label", (Identifier))
ERROR(parameter_operator_keyword_argument,none,
      "%select{operator|closure|enum case}0 cannot have keyword arguments",
      (unsigned))

ERROR(parameter_unnamed,none,
      "unnamed parameters must be written with the empty name '_'", ())
WARNING(parameter_unnamed_warn,none,
        "unnamed parameters must be written with the empty name '_'", ())

ERROR(parameter_curry_syntax_removed,none,
      "cannot have more than one parameter list", ())

ERROR(initializer_as_typed_pattern,none,
      "unexpected initializer in pattern; did you mean to use '='?", ())

ERROR(unlabeled_parameter_following_variadic_parameter,none,
      "a parameter following a variadic parameter requires a label", ())

ERROR(enum_element_empty_arglist,none,
      "enum element with associated values must have at least one "
      "associated value", ())
WARNING(enum_element_empty_arglist_swift4,none,
        "enum element with associated values must have at least one "
        "associated value; this will be an error in the future "
        "version of Swift", ())
NOTE(enum_element_empty_arglist_delete,none,
     "did you mean to remove the empty associated value list?", ())
NOTE(enum_element_empty_arglist_add_void,none,
     "did you mean to explicitly add a 'Void' associated value?", ())

//------------------------------------------------------------------------------
// Statement parsing diagnostics
//------------------------------------------------------------------------------
ERROR(expected_stmt,none,
      "expected statement", ())
ERROR(illegal_top_level_stmt,none,
      "statements are not allowed at the top level", ())
ERROR(illegal_top_level_expr,none,
      "expressions are not allowed at the top level", ())
ERROR(illegal_semi_stmt,none,
      "';' statements are not allowed", ())
ERROR(statement_begins_with_closure,none,
      "top-level statement cannot begin with a closure expression", ())
ERROR(statement_same_line_without_semi,none,
      "consecutive statements on a line must be separated by ';'", ())
ERROR(invalid_label_on_stmt,none,
      "labels are only valid on loops, if, and switch statements", ())

ERROR(snake_case_deprecated,none,
      "%0 has been replaced with %1 in Swift 3",
      (StringRef, StringRef))


// Assignment statement
ERROR(expected_expr_assignment,none,
      "expected expression in assignment", ())

// Brace Statement
ERROR(expected_rbrace_in_brace_stmt,none,
      "expected '}' at end of brace statement", ())

/// Associatedtype Statement
ERROR(typealias_inside_protocol_without_type,none,
      "type alias is missing an assigned type; use 'associatedtype' to define an associated type requirement", ())
ERROR(associatedtype_outside_protocol,none,
      "associated types can only be defined in a protocol; define a type or introduce a 'typealias' to satisfy an associated type requirement", ())

// Return Statement
ERROR(expected_expr_return,PointsToFirstBadToken,
      "expected expression in 'return' statement", ())
WARNING(unindented_code_after_return,none,
        "expression following 'return' is treated as an argument of "
        "the 'return'", ())
NOTE(indent_expression_to_silence,none,
     "indent the expression to silence this warning", ())

// Throw Statement
ERROR(expected_expr_throw,PointsToFirstBadToken,
      "expected expression in 'throw' statement", ())

// Yield Statment
ERROR(expected_expr_yield,PointsToFirstBadToken,
      "expected expression in 'yield' statement", ())

// Defer Statement
ERROR(expected_lbrace_after_defer,PointsToFirstBadToken,
      "expected '{' after 'defer'", ())

// If/While/Guard Conditions
ERROR(expected_comma_stmtcondition,none,
      "expected ',' joining parts of a multi-clause condition", ())

ERROR(expected_expr_conditional,PointsToFirstBadToken,
      "expected expression in conditional", ())

ERROR(expected_binding_keyword,none,
      "expected '%0' in conditional", (StringRef))

ERROR(expected_expr_conditional_var,PointsToFirstBadToken,
      "expected expression after '=' in conditional binding", ())
ERROR(conditional_var_initializer_required,none,
      "variable binding in a condition requires an initializer", ())
ERROR(wrong_condition_case_location,none,
      "pattern matching binding is spelled with 'case %0', not '%0 case'",
      (StringRef))

// If Statement
ERROR(expected_condition_if,PointsToFirstBadToken,
      "expected expression, var, or let in 'if' condition", ())
ERROR(missing_condition_after_if,none,
      "missing condition in an 'if' statement", ())
ERROR(expected_lbrace_after_if,PointsToFirstBadToken,
      "expected '{' after 'if' condition", ())
ERROR(expected_lbrace_or_if_after_else,PointsToFirstBadToken,
      "expected '{' or 'if' after 'else'", ())
ERROR(expected_lbrace_or_if_after_else_fixit,PointsToFirstBadToken,
      "expected '{' or 'if' after 'else'; did you mean to write 'if'?", ())
ERROR(unexpected_else_after_if,none,
      "unexpected 'else' immediately following 'if' condition", ())
NOTE(suggest_removing_else,none,
     "remove 'else' to execute the braced block of statements when the condition is true", ())

// Guard Statement
ERROR(expected_condition_guard,PointsToFirstBadToken,
      "expected expression, var, let or case in 'guard' condition", ())
ERROR(missing_condition_after_guard,none,
      "missing condition in an 'guard' statement", ())
ERROR(expected_else_after_guard,PointsToFirstBadToken,
      "expected 'else' after 'guard' condition", ())
ERROR(expected_lbrace_after_guard,PointsToFirstBadToken,
      "expected '{' after 'guard' else", ())
ERROR(bound_var_guard_body,none,
      "variable declared in 'guard' condition is not usable in its body", ())

// While Statement
ERROR(expected_condition_while,PointsToFirstBadToken,
      "expected expression, var, or let in 'while' condition", ())
ERROR(missing_condition_after_while,none,
      "missing condition in a 'while' statement", ())
ERROR(expected_lbrace_after_while,PointsToFirstBadToken,
      "expected '{' after 'while' condition", ())

// Repeat/While Statement
ERROR(expected_lbrace_after_repeat,PointsToFirstBadToken,
      "expected '{' after 'repeat'", ())
ERROR(expected_while_after_repeat_body,PointsToFirstBadToken,
      "expected 'while' after body of 'repeat' statement", ())
ERROR(expected_expr_repeat_while,PointsToFirstBadToken,
      "expected expression in 'repeat-while' condition", ())

ERROR(do_while_now_repeat_while,none,
      "'do-while' statement is not allowed; use 'repeat-while' instead", ())

// Do/Catch Statement
ERROR(expected_lbrace_after_do,PointsToFirstBadToken,
      "expected '{' after 'do'", ())
ERROR(expected_lbrace_after_catch,PointsToFirstBadToken,
      "expected '{' after 'catch' pattern", ())
ERROR(expected_catch_where_expr,PointsToFirstBadToken,
      "expected expression for 'where' guard of 'catch'", ())
ERROR(docatch_not_trycatch,PointsToFirstBadToken,
      "the 'do' keyword is used to specify a 'catch' region",
      ())

// C-Style For Stmt
ERROR(c_style_for_stmt_removed,none,
      "C-style for statement has been removed in Swift 3", ())

// For-each Stmt
ERROR(expected_foreach_in,PointsToFirstBadToken,
      "expected 'in' after for-each pattern", ())
ERROR(expected_foreach_container,PointsToFirstBadToken,
      "expected Sequence expression for for-each loop", ())
ERROR(expected_foreach_lbrace,PointsToFirstBadToken,
      "expected '{' to start the body of for-each loop", ())
ERROR(expected_foreach_where_expr,PointsToFirstBadToken,
      "expected expression in 'where' guard of 'for/in'", ())

// Switch Stmt
ERROR(expected_switch_expr,PointsToFirstBadToken,
      "expected expression in 'switch' statement", ())
ERROR(expected_lbrace_after_switch,PointsToFirstBadToken,
      "expected '{' after 'switch' subject expression", ())
ERROR(expected_rbrace_switch,none,
      "expected '}' at end of 'switch' statement", ())
ERROR(case_outside_of_switch,none,
      "'%0' label can only appear inside a 'switch' statement", (StringRef))
ERROR(stmt_in_switch_not_covered_by_case,none,
      "all statements inside a switch must be covered by a 'case' or 'default'",
      ())
ERROR(case_after_default,none,
      "additional 'case' blocks cannot appear after the 'default' block of a 'switch'",
      ())

// Case Stmt
ERROR(expected_case_where_expr,PointsToFirstBadToken,
      "expected expression for 'where' guard of 'case'", ())
ERROR(expected_case_colon,PointsToFirstBadToken,
      "expected ':' after '%0'", (StringRef))
ERROR(default_with_where,none,
      "'default' cannot be used with a 'where' guard expression",
      ())
ERROR(case_stmt_without_body,none,
      "%select{'case'|'default'}0 label in a 'switch' should have at least one "
      "executable statement", (bool))

// 'try' on statements
ERROR(try_on_stmt,none,
      "'try' cannot be used with '%0'", (StringRef))
ERROR(try_on_return_throw_yield,none,
      "'try' must be placed on the %select{returned|thrown|yielded}0 expression",
      (unsigned))
ERROR(try_on_var_let,none,
      "'try' must be placed on the initial value expression", ())

//------------------------------------------------------------------------------
// MARK: Expression parsing diagnostics
//------------------------------------------------------------------------------

ERROR(expected_expr,none,
      "expected expression", ())
ERROR(expected_separator,PointsToFirstBadToken,
      "expected '%0' separator", (StringRef))
ERROR(unexpected_separator,none,
      "unexpected '%0' separator", (StringRef))

ERROR(expected_expr_after_operator,none,
      "expected expression after operator", ())
ERROR(expected_expr_after_unary_operator,none,
      "expected expression after unary operator", ())
ERROR(expected_prefix_operator,none,
      "unary operator cannot be separated from its operand", ())
ERROR(expected_operator_ref,none,
      "expected operator name in operator reference", ())
ERROR(invalid_postfix_operator,none,
      "operator with postfix spacing cannot start a subexpression", ())

ERROR(expected_member_name,PointsToFirstBadToken,
      "expected member name following '.'", ())
ERROR(dollar_numeric_too_large,none,
      "numeric value following '$' is too large", ())
ERROR(numeric_literal_numeric_member,none,
      "expected named member of numeric literal", ())
ERROR(standalone_dollar_identifier,none,
     "'$' is not an identifier; use backticks to escape it", ())
ERROR(dollar_identifier_decl,none,
     "cannot declare entity named %0; the '$' prefix is reserved for "
     "implicitly-synthesized declarations", (Identifier))

ERROR(anon_closure_arg_not_in_closure,none,
      "anonymous closure argument not contained in a closure", ())
ERROR(anon_closure_arg_in_closure_with_args,none,
      "anonymous closure arguments cannot be used inside a closure that has "
      "explicit arguments", ())
ERROR(anon_closure_arg_in_closure_with_args_typo,none,
      "anonymous closure arguments cannot be used inside a closure that has "
      "explicit arguments; did you mean '%0'?", (StringRef))
ERROR(anon_closure_tuple_param_destructuring,none,
      "closure tuple parameter does not support destructuring", ())
ERROR(expected_closure_parameter_name,none,
      "expected the name of a closure parameter", ())
ERROR(expected_capture_specifier,none,
      "expected 'weak', 'unowned', or no specifier in capture list", ())
ERROR(expected_capture_specifier_name,none,
      "expected name of in closure capture list", ())
ERROR(expected_init_capture_specifier,none,
      "expected initializer for closure capture specifier", ())
ERROR(expected_capture_list_end_rsquare,none,
      "expected ']' at end of capture list", ())
ERROR(cannot_capture_fields,none,
      "fields may only be captured by assigning to a specific name", ())

ERROR(expected_closure_result_type,none,
      "expected closure result type after '->'", ())
ERROR(expected_closure_in,none,
   "expected 'in' after the closure signature", ())
ERROR(unexpected_tokens_before_closure_in,none,
   "unexpected tokens prior to 'in'", ())
ERROR(expected_closure_rbrace,none,
      "expected '}' at end of closure", ())

WARNING(trailing_closure_after_newlines,none,
        "braces here form a trailing closure separated from its callee by multiple newlines", ())
NOTE(trailing_closure_callee_here,none,
     "callee is here", ())

ERROR(string_literal_no_atsign,none,
      "string literals in Swift are not preceded by an '@' sign", ())

ERROR(invalid_float_literal_missing_leading_zero,none,
      "'.%0' is not a valid floating point literal; it must be written '0.%0'",
      (StringRef))
ERROR(availability_query_outside_if_stmt_guard, none,
      "#available may only be used as condition of an 'if', 'guard'"
      " or 'while' statement", ())

ERROR(empty_arg_label_underscore, none,
      "an empty argument label is spelled with '_'", ())

ERROR(expected_identifier_after_dot_expr,none,
      "expected identifier after '.' expression", ())

ERROR(expected_identifier_after_super_dot_expr,
      PointsToFirstBadToken,
      "expected identifier or 'init' after super '.' expression", ())
ERROR(expected_dot_or_subscript_after_super,PointsToFirstBadToken,
      "expected '.' or '[' after 'super'", ())
ERROR(super_in_closure_with_capture,none,
      "using 'super' in a closure where 'self' is explicitly captured is "
      "not yet supported", ())
NOTE(super_in_closure_with_capture_here,none,
     "'self' explicitly captured here", ())

// Tuples and parenthesized expressions
ERROR(expected_expr_in_expr_list,none,
      "expected expression in list of expressions", ())
ERROR(expected_expr_in_collection_literal,none,
      "expected expression in container literal", ())
ERROR(expected_key_in_dictionary_literal,none,
      "expected key expression in dictionary literal", ())
ERROR(expected_value_in_dictionary_literal,none,
      "expected value in dictionary literal", ())
ERROR(expected_colon_in_dictionary_literal,none,
      "expected ':' in dictionary literal", ())
ERROR(expected_rparen_expr_list,none,
      "expected ')' in expression list", ())
ERROR(expected_rsquare_expr_list,none,
      "expected ']' in expression list", ())

// Array literal expressions
ERROR(expected_rsquare_array_expr,PointsToFirstBadToken,
      "expected ']' in container literal expression", ())

// Object literal expressions
ERROR(expected_arg_list_in_object_literal,PointsToFirstBadToken,
      "expected argument list in object literal", ())
ERROR(legacy_object_literal,none,
      "'%select{|[}0#%1(...)%select{|#]}0' has been renamed to '#%2(...)'",
      (bool, StringRef, StringRef))

// Unknown pound expression.
ERROR(unknown_pound_expr,none,
      "use of unknown directive '#%0'", (StringRef))

// If expressions
ERROR(expected_expr_after_if_question,none,
      "expected expression after '?' in ternary expression", ())
ERROR(expected_colon_after_if_question,none,
      "expected ':' after '? ...' in ternary expression", ())
ERROR(expected_expr_after_if_colon,none,
      "expected expression after '? ... :' in ternary expression", ())

// Cast expressions
ERROR(expected_type_after_is,none,
      "expected type after 'is'", ())
ERROR(expected_type_after_as,none,
      "expected type after 'as'", ())

// Extra tokens in string interpolation like in " >> \( $0 } ) << "
ERROR(string_interpolation_extra,none,
      "extra tokens after interpolated string expression", ())

// Interpolations with parameter labels or multiple values
WARNING(string_interpolation_list_changing,none,
      "interpolating multiple values will not form a tuple in Swift 5", ())
NOTE(string_interpolation_list_insert_parens,none,
      "insert parentheses to keep current behavior", ())
WARNING(string_interpolation_label_changing,none,
      "labeled interpolations will not be ignored in Swift 5", ())
NOTE(string_interpolation_remove_label,none,
      "remove %0 label to keep current behavior", (Identifier))

// Keypath expressions.
ERROR(expr_keypath_expected_lparen,PointsToFirstBadToken,
      "expected '(' following '#keyPath'", ())
ERROR(expr_keypath_expected_property_or_type,PointsToFirstBadToken,
      "expected property or type name within '#keyPath(...)'", ())
ERROR(expr_keypath_expected_rparen,PointsToFirstBadToken,
      "expected ')' to complete '#keyPath' expression", ())
ERROR(expr_keypath_expected_expr,none,
      "expected expression path in Swift key path",())

// Selector expressions.
ERROR(expr_selector_expected_lparen,PointsToFirstBadToken,
      "expected '(' following '#selector'", ())
ERROR(expr_selector_expected_method_expr,PointsToFirstBadToken,
      "expected expression naming a method within '#selector(...)'", ())
ERROR(expr_selector_expected_property_expr,PointsToFirstBadToken,
      "expected expression naming a property within '#selector(...)'", ())
ERROR(expr_selector_expected_rparen,PointsToFirstBadToken,
      "expected ')' to complete '#selector' expression", ())

// Type-of expressions.
ERROR(expr_dynamictype_deprecated,PointsToFirstBadToken,
      "'.dynamicType' is deprecated. Use 'type(of: ...)' instead", ())

ERROR(pound_assert_disabled,PointsToFirstBadToken,
      "#assert is an experimental feature that is currently disabled", ())
ERROR(pound_assert_expected_lparen,PointsToFirstBadToken,
      "expected '(' in #assert directive", ())
ERROR(pound_assert_expected_rparen,PointsToFirstBadToken,
      "expected ')' in #assert directive", ())
ERROR(pound_assert_expected_expression,PointsToFirstBadToken,
      "expected a condition expression", ())
ERROR(pound_assert_expected_string_literal,PointsToFirstBadToken,
      "expected a string literal", ())

//------------------------------------------------------------------------------
// MARK: Attribute-parsing diagnostics
//------------------------------------------------------------------------------

ERROR(replace_equal_with_colon_for_value,none,
      "'=' has been replaced with ':' in attribute arguments", ())
ERROR(expected_attribute_name,none,
      "expected an attribute name", ())
ERROR(unknown_attribute,none,
      "unknown attribute '%0'", (StringRef))
ERROR(unexpected_lparen_in_attribute,none,
      "unexpected '(' in attribute '%0'", (StringRef))
ERROR(duplicate_attribute,none,
      "duplicate %select{attribute|modifier}0", (bool))
NOTE(previous_attribute,none,
     "%select{attribute|modifier}0 already specified here", (bool))
ERROR(mutually_exclusive_attrs,none,
      "'%0' contradicts previous %select{attribute|modifier}2 '%1'", (StringRef, StringRef, bool))

ERROR(invalid_infix_on_func,none,
      "'infix' modifier is not required or allowed on func declarations", ())

ERROR(expected_in_attribute_list,none,
      "expected ']' or ',' in attribute list", ())

ERROR(type_attribute_applied_to_decl,none,
      "attribute can only be applied to types, not declarations", ())
ERROR(decl_attribute_applied_to_type,none,
      "attribute can only be applied to declarations, not types", ())

ERROR(attr_expected_lparen,none,
"expected '(' in '%0' %select{attribute|modifier}1", (StringRef, bool))

ERROR(attr_expected_rparen,none,
"expected ')' in '%0' %select{attribute|modifier}1", (StringRef, bool))

ERROR(attr_expected_comma,none,
"expected ',' in '%0' %select{attribute|modifier}1", (StringRef, bool))

ERROR(attr_expected_string_literal,none,
"expected string literal in '%0' attribute", (StringRef))

ERROR(alignment_must_be_positive_integer,none,
      "alignment value must be a positive integer literal", ())

ERROR(swift_native_objc_runtime_base_must_be_identifier,none,
      "@_swift_native_objc_runtime_base class name must be an identifier", ())

ERROR(objc_runtime_name_must_be_identifier,none,
      "@_objcRuntimeName name must be an identifier", ())

ERROR(attr_only_at_non_local_scope, none,
      "attribute '%0' can only be used in a non-local scope", (StringRef))

ERROR(projection_value_property_not_identifier,none,
      "@_projectedValueProperty name must be an identifier", ())

// Access control
ERROR(attr_access_expected_set,none,
      "expected 'set' as subject of '%0' modifier", (StringRef))

// Attributes
ERROR(attr_renamed, none,
      "'@%0' has been renamed to '@%1'", (StringRef, StringRef))
WARNING(attr_renamed_warning, none,
        "'@%0' has been renamed to '@%1'", (StringRef, StringRef))
ERROR(attr_name_close_match, none,
      "no attribute named '@%0'; did you mean '@%1'?", (StringRef, StringRef))
ERROR(attr_unsupported_on_target, none,
      "attribute '%0' is unsupported on target '%1'", (StringRef, StringRef))

// availability
ERROR(attr_availability_platform,none,
      "expected platform name or '*' for '%0' attribute", (StringRef))
ERROR(attr_availability_unavailable_deprecated,none,
      "'%0' attribute cannot be both unconditionally 'unavailable' and "
      "'deprecated'", (StringRef))

WARNING(attr_availability_invalid_duplicate,none,
        "'%0' argument has already been specified", (StringRef))
WARNING(attr_availability_unknown_platform,none,
      "unknown platform '%0' for attribute '%1'", (StringRef, StringRef))
ERROR(attr_availability_invalid_renamed,none,
      "'renamed' argument of '%0' attribute must be an operator, identifier, "
      "or full function name, optionally prefixed by a type name", (StringRef))

ERROR(attr_availability_expected_option,none,
      "expected '%0' option such as 'unavailable', 'introduced', 'deprecated', "
      "'obsoleted', 'message', or 'renamed'", (StringRef))

ERROR(attr_availability_expected_equal,none,
      "expected ':' after '%1' in '%0' attribute", (StringRef, StringRef))

ERROR(attr_availability_expected_version,none,
      "expected version number in '%0' attribute", (StringRef))

WARNING(attr_availability_platform_agnostic_expected_option,none,
      "expected 'introduced', 'deprecated', or 'obsoleted' in '%0' attribute "
      "for platform '%1'", (StringRef, StringRef))
WARNING(attr_availability_platform_agnostic_expected_deprecated_version,none,
      "expected version number with 'deprecated' in '%0' attribute for "
      "platform '%1'", (StringRef, StringRef))
WARNING(attr_availability_platform_agnostic_infeasible_option,none,
      "'%0' cannot be used in '%1' attribute for platform '%2'",
      (StringRef, StringRef, StringRef))

WARNING(attr_availability_nonspecific_platform_unexpected_version,none,
      "unexpected version number in '%0' attribute for non-specific platform "
      "'*'", (StringRef))

// originallyDefinedIn
ERROR(originally_defined_in_missing_rparen,none,
      "expected ')' in @_originallyDefinedIn argument list", ())

ERROR(originally_defined_in_unrecognized_platform,none,
      "unrecognized platform name in @_originallyDefinedIn argument list", ())

ERROR(originally_defined_in_unrecognized_property,none,
      "unrecognized property in @_originallyDefinedIn argument list", ())

ERROR(originally_defined_in_need_original_module_name,none,
      "expected 'module: \"original\"' in the first argument to "
      "@_originallyDefinedIn", ())

ERROR(originally_defined_in_need_nonempty_module_name,none,
      "original module name cannot be empty in @_originallyDefinedIn", ())

ERROR(originally_defined_in_need_platform_version,none,
     "expected at least one platform version in @_originallyDefinedIn", ())

WARNING(originally_defined_in_major_minor_only,none,
        "@_originallyDefinedIn only uses major and minor version number", ())

WARNING(originally_defined_in_missing_platform_name,none,
        "* as platform name has no effect", ())

// convention
ERROR(convention_attribute_expected_lparen,none,
      "expected '(' after 'convention' attribute", ())
ERROR(convention_attribute_expected_name,none,
      "expected convention name identifier in 'convention' attribute", ())
ERROR(convention_attribute_expected_rparen,none,
      "expected ')' after convention name for 'convention' attribute", ())
ERROR(convention_attribute_witness_method_expected_colon,none,
      "expected ':' after 'witness_method' for 'convention' attribute", ())
ERROR(convention_attribute_witness_method_expected_protocol,none,
      "expected protocol name in 'witness_method' 'convention' attribute", ())

// objc
ERROR(attr_objc_missing_colon,none,
      "missing ':' after selector piece in @objc attribute", ())
ERROR(attr_objc_expected_rparen,none,
      "expected ')' after name for @objc", ())
ERROR(attr_objc_empty_name,none,
      "expected name within parentheses of @objc attribute", ())

ERROR(attr_dynamic_replacement_expected_rparen,none,
      "expected ')' after function name for @_dynamicReplacement", ())
ERROR(attr_dynamic_replacement_expected_function,none,
      "expected a function name in @_dynamicReplacement(for:)", ())
ERROR(attr_dynamic_replacement_expected_for,none,
      "expected 'for' in '_dynamicReplacement' attribute", ())
ERROR(attr_dynamic_replacement_expected_colon,none,
      "expected ':' after @_dynamicReplacement(for", ())

ERROR(attr_private_import_expected_rparen,none,
      "expected ')' after function name for @_private", ())
ERROR(attr_private_import_expected_sourcefile, none,
      "expected 'sourceFile' in '_private' attribute", ())
ERROR(attr_private_import_expected_sourcefile_name,none,
      "expected a source file name in @_private(sourceFile:)", ())
ERROR(attr_private_import_expected_colon,none,
      "expected ':' after @_private(sourceFile", ())

// opened
ERROR(opened_attribute_expected_lparen,none,
      "expected '(' after 'opened' attribute", ())
ERROR(opened_attribute_id_value,none,
      "known id for 'opened' attribute must be a UUID string", ())
ERROR(opened_attribute_expected_rparen,none,
      "expected ')' after id value for 'opened' attribute", ())

// inline, optimize
ERROR(optimization_attribute_expect_option,none,
      "expected '%0' option such as '%1'", (StringRef, StringRef))
ERROR(optimization_attribute_unknown_option,none,
      "unknown option '%0' for attribute '%1'", (StringRef, StringRef))

// effects
ERROR(effects_attribute_expect_option,none,
      "expected '%0' option (readnone, readonly, readwrite)", (StringRef))
ERROR(effects_attribute_unknown_option,none,
      "unknown option '%0' for attribute '%1'", (StringRef, StringRef))

// unowned
ERROR(attr_unowned_invalid_specifier,none,
      "expected 'safe' or 'unsafe'", ())
ERROR(attr_unowned_expected_rparen,none,
      "expected ')' after specifier for 'unowned'", ())

// warn_unused_result
WARNING(attr_warn_unused_result_removed,none,
        "'warn_unused_result' attribute behavior is now the default", ())
ERROR(attr_warn_unused_result_expected_rparen,none,
      "expected ')' after 'warn_unused_result' attribute", ())

// _specialize
ERROR(attr_specialize_missing_colon,none,
      "missing ':' after %0 in '_specialize' attribute", (StringRef))

ERROR(attr_specialize_missing_comma,none,
      "missing ',' in '_specialize' attribute", ())

ERROR(attr_specialize_unknown_parameter_name,none,
      "unknown parameter %0 in '_specialize attribute'", (StringRef))

ERROR(attr_specialize_expected_bool_value,none,
      "expected a boolean true or false value in '_specialize' attribute", ())

ERROR(attr_specialize_missing_parameter_label_or_where_clause,none,
      "expected a parameter label or a where clause in '_specialize' attribute", ())

ERROR(attr_specialize_parameter_already_defined,none,
      "parameter '%0' was already defined in '_specialize' attribute", (StringRef))

ERROR(attr_specialize_expected_partial_or_full,none,
      "expected 'partial' or 'full' as values of the 'kind' parameter in '_specialize' attribute", ())

// _implements
ERROR(attr_implements_expected_member_name,PointsToFirstBadToken,
      "expected a member name as second parameter in '_implements' attribute", ())

// differentiable
ERROR(attr_differentiable_expected_function_name,PointsToFirstBadToken,
      "expected a %0 function name", (StringRef))
ERROR(attr_differentiable_expected_parameter_list,PointsToFirstBadToken,
      "expected a list of parameters to differentiate with respect to", ())
ERROR(attr_differentiable_use_wrt_not_withrespectto,none,
      "use 'wrt:' to specify parameters to differentiate with respect to", ())
ERROR(attr_differentiable_missing_label,PointsToFirstBadToken,
      "missing label '%0:' in '@differentiable' attribute", (StringRef))
ERROR(attr_differentiable_expected_label,none,
      "expected either 'wrt:' or a function specifier label, e.g. 'jvp:', "
      "or 'vjp:'", ())
ERROR(differentiable_attribute_expected_rparen,none,
      "expected ')' in '@differentiable' attribute", ())
ERROR(unexpected_argument_differentiable,none,
      "unexpected argument '%0' in '@differentiable' attribute", (StringRef))

// differentiation `wrt` parameters clause
ERROR(expected_colon_after_label,PointsToFirstBadToken,
      "expected a colon ':' after '%0'", (StringRef))
ERROR(diff_params_clause_expected_parameter,PointsToFirstBadToken,
      "expected a parameter, which can be a function parameter name, "
      "parameter index, or 'self'", ())

//------------------------------------------------------------------------------
// MARK: Generics parsing diagnostics
//------------------------------------------------------------------------------
ERROR(expected_rangle_generics_param,PointsToFirstBadToken,
      "expected '>' to complete generic parameter list", ())
ERROR(expected_generics_parameter_name,PointsToFirstBadToken,
      "expected an identifier to name generic parameter", ())
ERROR(unexpected_class_constraint,none,
       "'class' constraint can only appear on protocol declarations", ())
NOTE(suggest_anyobject,none,
     "did you mean to write an 'AnyObject' constraint?", ())
ERROR(expected_generics_type_restriction,none,
      "expected a class type or protocol-constrained type restricting %0",
      (Identifier))
ERROR(requires_single_equal,none,
      "use '==' for same-type requirements rather than '='", ())
ERROR(requires_comma,none,
      "expected ',' to separate the requirements of this 'where' clause", ())
ERROR(expected_requirement_delim,none,
      "expected ':' or '==' to indicate a conformance or same-type requirement",
      ())
ERROR(redundant_class_requirement,none,
      "redundant 'class' requirement", ())
ERROR(late_class_requirement,none,
      "'class' must come first in the requirement list", ())
ERROR(where_without_generic_params,none,
      "'where' clause cannot be attached to "
      "%select{a non-generic|a protocol|an associated type}0 "
      "declaration", (unsigned))
ERROR(where_inside_brackets,none,
        "'where' clause next to generic parameters is obsolete, "
        "must be written following the declaration's type", ())

//------------------------------------------------------------------------------
// MARK: Conditional compilation parsing diagnostics
//------------------------------------------------------------------------------
ERROR(unsupported_conditional_compilation_binary_expression,none,
      "expected '&&' or '||' expression", ())
ERROR(unsupported_conditional_compilation_unary_expression,none,
      "expected unary '!' expression", ())
ERROR(unsupported_platform_condition_expression,none,
      "unexpected platform condition "
      "(expected 'os', 'arch', or 'swift')",
      ())
ERROR(platform_condition_expected_one_argument,none,
      "expected only one argument to platform condition",
      ())
ERROR(unsupported_platform_runtime_condition_argument,none,
      "unexpected argument for the '_runtime' condition; "
      "expected '_Native' or '_ObjC'", ())
ERROR(unsupported_platform_condition_argument,none,
      "unexpected platform condition argument: expected %0",
      (StringRef))
ERROR(unsupported_conditional_compilation_expression_type,none,
      "invalid conditional compilation expression", ())
ERROR(unsupported_conditional_compilation_integer,none,
      "'%0' is not a valid conditional compilation expression, use '%1'",
      (StringRef, StringRef))
ERROR(version_component_not_number,none,
      "version component contains non-numeric characters", ())
ERROR(compiler_version_too_many_components,none,
      "compiler version must not have more than five components", ())
WARNING(unused_compiler_version_component,none,
      "the second version component is not used for comparison", ())
ERROR(empty_version_component,none,
      "found empty version component", ())
ERROR(compiler_version_component_out_of_range,none,
      "compiler version component out of range: must be in [0, %0]",
      (unsigned))
ERROR(empty_version_string,none,
      "version requirement is empty", ())
WARNING(unknown_platform_condition_argument,none,
      "unknown %0 for build configuration '%1'",
      (StringRef, StringRef))
WARNING(likely_simulator_platform_condition,none,
      "platform condition appears to be testing for simulator environment; "
      "use 'targetEnvironment(simulator)' instead",
      ())

//------------------------------------------------------------------------------
// MARK: Availability query parsing diagnostics
//------------------------------------------------------------------------------
ERROR(avail_query_expected_condition,PointsToFirstBadToken,
      "expected availability condition", ())
ERROR(avail_query_expected_platform_name,PointsToFirstBadToken,
      "expected platform name", ())

ERROR(avail_query_expected_version_number,PointsToFirstBadToken,
      "expected version number", ())
ERROR(avail_query_expected_rparen,PointsToFirstBadToken,
      "expected ')' in availability query", ())

WARNING(avail_query_unrecognized_platform_name,
        PointsToFirstBadToken, "unrecognized platform name %0", (Identifier))

ERROR(avail_query_disallowed_operator, PointsToFirstBadToken,
      "'%0' cannot be used in an availability condition", (StringRef))

ERROR(avail_query_argument_and_shorthand_mix_not_allowed, PointsToFirstBadToken,
      "'%0' can't be combined with shorthand specification '%1'",
      (StringRef, StringRef))

NOTE(avail_query_meant_introduced,PointsToFirstBadToken,
     "did you mean to specify an introduction version?", ())

ERROR(avail_query_version_comparison_not_needed,
      none,"version comparison not needed", ())

ERROR(availability_query_wildcard_required, none,
      "must handle potential future platforms with '*'", ())

ERROR(availability_must_occur_alone, none,
      "'%0' version-availability must be specified alone", (StringRef))

ERROR(pound_available_swift_not_allowed, none,
      "Swift language version checks not allowed in #available(...)", ())

ERROR(pound_available_package_description_not_allowed, none,
      "PackageDescription version checks not allowed in #available(...)", ())

ERROR(availability_query_repeated_platform, none,
      "version for '%0' already specified", (StringRef))

//------------------------------------------------------------------------------
// MARK: syntax parsing diagnostics
//------------------------------------------------------------------------------
ERROR(unknown_syntax_entity, PointsToFirstBadToken,
      "unknown %0 syntax exists in the source", (StringRef))

#ifndef DIAG_NO_UNDEF
# if defined(DIAG)
#  undef DIAG
# endif
# undef NOTE
# undef WARNING
# undef ERROR
# undef FIXIT
#endif
