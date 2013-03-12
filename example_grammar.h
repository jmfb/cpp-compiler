#
# translation-unit
#
<translation-unit> =
	<declaration-list-opt>;

<declaration-list> =
	<declaration> <declaration-list-opt>;

<declaration> =
	<block-declaration> |
	#<function-definition> |
	#<template-declaration> |
	#<explicit-instantiation> |
	#<explicit-specialization> |
	#<attribute-declaration> |
	<linkage-specification> |
	<namespace-definition> |
	<empty-declaration>;

<block-declaration> =
	#<simple-declaration> |
	#<asm-definition> |
	#<namespace-alias-definition> |
	#<using-declaration> |
	#<using-directive> |
	#<alias-declaration> |
	#<opaque-enum-declaration> |
	<static-assert-declaration>;

<static-assert-declaration> =
	"static_assert" '(' <constant-expression> ',' val ')' ';';

#
# decl-specifier
#
<decl-specifier-list> =
	<decl-specifier> <decl-specifier-list-opt>;
<decl-specifier> =
	<storage-class-specifier> |
	<type-specifier> |
	<function-specifier> |
	"friend" |
	"typeof" |
	"constexpr";
<storage-class-specifier> =
	"auto" |
	"register" |
	"static" |
	"thread_local" |
	"extern" |
	"mutable";
<function-specifier> =
	"inline" |
	"virtual" |
	"explicit";
<type-specifier> =
	#<trailing-type-specifier> |
	#<class-specifier> |
	<enum-specifier>;
<enum-specifier> =
	"enum" <op-{-@push(null)> <enum-body> <op-}-@pop> |
	"enum" <id=class-name> <op-{-@push(null)> <enum-body> <op-}-@pop> |
	"enum" <class-key> <id=class-name,$class-name> <op-{-@push($class-name)> <enum-body> <op-}-@pop>;
<enum-body> =
	<enumerator-list-opt> |
	<enumerator-list> ',';
<enumerator-list> =
	<enumerator-definition> |
	<enumerator-list> ',' <enumerator-definition>;
<enumerator-definition> =
	<enumerator> |
	<enumerator> '=' <constant-expression>;
<enumerator> =
	<id=enumerator-name>;
<class-key> =
	"class" |
	"struct";

#
# expressions
#
<constant-expression> =
	val;

#
# linkage-specification
#
<linkage-specification> =
	"extern" val <op-{-@push(null)> <declaration-list-opt> <op-}-@pop> |
	"extern" val <declaration>;

#
# namespace-definition
#
<namespace-definition> =
	<named-namespace-definition> |
	<unnamed-namespace-definition>;
<named-namespace-definition> =
	<key-inline-opt>
	"namespace"
	<id*,$namespace-name>
	<op-{-@defpush(namespace-name,$namespace-name)>
	<namespace-body>
	<op-}-@pop>;
<unnamed-namespace-definition> =
	<key-inline-opt>
	"namespace"
	<op-{-@push(unnamed)>
	<namespace-body>
	<op-}-@pop>;
<namespace-body> =
	<declaration-list-opt>;

#
# empty-declaration
#
<empty-declaration> =
	';';

