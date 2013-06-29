<declaration> =
	<class-declaration> |
	<enum-declaration>;

<class-declaration> =
	"class" id '{' <class-member-list-opt> '}' |
	"class" id ':' <class-base-list> '{' <class-member-list-opt> '}';
<class-base-list> =
	<class-base> |
	<class-base> ',' <class-base-list>;
<class-base> =
	"virtual" <access-modifier> <qualified-name> |
	<access-modifier> <qualified-name>;
<class-member-list> =
	<class-member> <class-member-list-opt>;
<class-member> =
	<class-member-variable> |
	<class-member-constructor> |
	<class-member-destructor> |
	<class-member-function> |
	<class-member-property>;

<class-member-access> =
	<access-modifier> |
	<access-modifier> "static" |
	<access-modifier> "virtual";
<access-modifier> =
	"public" |
	"protected" |
	"private";

<class-member-variable> =
	<class-member-access>
	<type-name>
	id
	<variable-initializer-opt>
	';';
<variable-initializer> =
	'=' <expression>;

<class-member-constructor> =
	<class-member-access> id '(' <argument-list-opt> ')' '{' <statement-list-opt> '}' |
	<class-member-access> id '(' <argument-list-opt> ')' ':' <class-member-initializer-list> '{' <statement-list-opt> '}' |
	<class-member-access> id '(' <argument-list-opt> ')' '=' "default" ';' |
	<class-member-access> id '(' <argument-list-opt> ')' '=' "delete" ';';
<class-member-initializer-list> =
	<qualified-name> '(' <expression-list-opt> ')' |
	<qualified-name> '(' <expression-list-opt> ')' ',' <class-member-initializer-list>;

<class-member-destructor> =
	<class-member-access> '~' id '(' ')' '{' <statement-list-opt> '}';

<class-member-function> =
	<class-member-access> <type-name> id '(' <argument-list-opt> ')' "const" '{' <statement-list-opt> '}' |
	<class-member-access> <type-name> id '(' <argument-list-opt> ')' "const" '=' val ';' |
	<class-member-access> <type-name> id '(' <argument-list-opt> ')' "const" '=' "default" ';' |
	<class-member-access> <type-name> id '(' <argument-list-opt> ')' "const" '=' "delete" ';' |
	<class-member-access> <type-name> id '(' <argument-list-opt> ')' '{' <statement-list-opt> '}' |
	<class-member-access> <type-name> id '(' <argument-list-opt> ')' '=' val ';' |
	<class-member-access> <type-name> id '(' <argument-list-opt> ')' '=' "default" ';' |
	<class-member-access> <type-name> id '(' <argument-list-opt> ')' '=' "delete" ';';

<class-member-property> =
	<class-member-access> <type-name> id '{' <class-member-property-function-list> '}' |
	<class-member-access> <type-name> id '{' <class-member-property-function-list> '}' <variable-initializer> ';';
<class-member-property-function-list> =
	<class-member-property-function> <class-member-property-function-list-opt>;
<class-member-property-function> =
	<access-modifier-opt> <class-member-property-type> ';' |
	<access-modifier-opt> <class-member-property-type> '{' <statement-list-opt> '}';
<class-member-property-type> =
	"get" |
	"getcopy" |
	"getref" |
	"set" |
	"setcopy";

<argument-list> =
	<argument> |
	<argument> ',' <argument-list>;
<argument> =
	<type-name> id <variable-initializer-opt>;

<enum-declaration> =
	"enum" id '{' <enum-member-list-opt> '}';
<enum-member-list> =
	<enum-member> |
	<enum-member> ',' <enum-member-list>;
<enum-member> =
	id |
	id '=' <expression>;

<expression-list> =
	<expression> |
	<expression> ',' <expression-list>;
<expression> =
	<assignment-expression>;
<assignment-expression> =
	<conditional-expression> |
	<isnull-expression> <assignment-operator> <assignment-expression>;
	#TODO: initializer clause, throw expression
<assignment-operator> =
	'=' |
	'*=' |
	'/=' |
	'%=' |
	'+=' |
	'-=' |
	'>>=' |
	'<<=' |
	'&=' |
	'^=' |
	'|=' |
	'?=';

<conditional-expression> =
	<isnull-expression> |
	<isnull-expression> '?' <expression> ':' <assignment-expression>;
<isnull-expression> =
	<logical-or-expression> |
	<isnull-expression> '??' <logical-or-expression>;
<logical-or-expression> =
	<logical-and-expression> |
	<logical-or-expression> '||' <logical-and-expression>;
<logical-and-expression> =
	<inclusive-or-expression> |
	<logical-and-expression> '&&' <inclusive-or-expression>;
<inclusive-or-expression> =
	<exclusive-or-expression> |
	<inclusive-or-expression> '|' <exclusive-or-expression>;
<exclusive-or-expression> =
	<and-expression> |
	<exclusive-or-expression> '^' <and-expression>;
<and-expression> =
	<equality-expression> |
	<and-expression> '&' <equality-expression>;
<equality-expression> =
	<relational-expression> |
	<equality-expression> '==' <relational-expression> |
	<equality-expression> '!=' <relational-expression>;
<relational-expression> =
	<shift-expression> |
	<relational-expression> '<' <shift-expression> |
	<relational-expression> '>' <shift-expression> |
	<relational-expression> '<=' <shift-expression> |
	<relational-expression> '>=' <shift-expression>;
<shift-expression> =
	<additive-expression> |
	<shift-expression> '<<' <additive-expression> |
	<shift-expression> '>>' <additive-expression>;
<additive-expression> =
	<multiplicative-expression> |
	<additive-expression> '+' <multiplicative-expression> |
	<additive-expression> '-' <multiplicative-expression>;
<multiplicative-expression> =
	<unary-expression> |
	<multiplicative-expression> '*' <unary-expression> |
	<multiplicative-expression> '/' <unary-expression> |
	<multiplicative-expression> '%' <unary-expression>;

<unary-expression> =
	<postfix-expression> |
	<unary-operator> <unary-expression>;
	#TODO: new, delete, sizeof
<unary-operator> =
	'++' |
	'--' |
	'*' |
	'&' |
	'+' |
	'-' |
	'!' |
	'~' |
	'@';

<postfix-expression> =
	<primary-expression> |
	<postfix-expression> '[' <expression> ']' |
	<postfix-expression> '(' <expression-list-opt> ')' |
	<postfix-expression> <member-access-operator> id |
	<postfix-expression> '++' |
	<postfix-expression> '--';
	#TODO: casts, typeid, spontaneous type initializations, etc.
<member-access-operator> =
	'.' |
	'.*' |
	'->' |
	'->*' |
	'?.' |
	'?.*' |
	'?->' |
	'?->*';

<primary-expression> =
	val |
	<qualified-name> |
	"true" |
	"false" |
	"null" |
	"this" |
	'(' <expression> ')';

<statement-list> =
	<statement> <statement-list-opt>;
<statement> =
	<expression-statement> |
	<return-statement>;
<expression-statement> =
	<expression> ';';
<return-statement> =
	"return" <expression-opt> ';';

#
# Everything below this point has been implemented in the code model (properly).
#

<translation-unit> =
	<using-list-opt> <namespace-declaration>;

<using-list> =
	<using-statement> <using-list-opt>;
<using-statement> =
	<using-import> |
	<using-alias>;
<using-import> =
	"using" <qualified-name> ';';
<using-alias> =
	"using" id '=' <qualified-name> ';';

<namespace-declaration> =
	"namespace" <qualified-name> '{' <declaration-list-opt> '}';
<declaration-list> =
	<declaration> <declaration-list-opt>;

<type-name> =
	<core-type> <pointer-specifier-opt> |
	"const" <core-type> <pointer-specifier-opt>;
<core-type> =
	<built-in-type> |
	<qualified-name>;
<built-in-type> =
	"var" |
	"void" |
	"bool" |
	"char" |
	"wchar" |
	"byte" |
	"ushort" |
	"short" |
	"uint" |
	"int" |
	"ulong" |
	"long" |
	"float" |
	"double" |
	"ldouble";
<pointer-specifier> =
	<pointer-operator> <pointer-specifier-opt> |
	<pointer-operator> "const" <pointer-specifier-opt>;
<pointer-operator> =
	'&' |
	'*' |
	'?';

<qualified-name> =
	id <qualified-suffix-opt>;
<qualified-suffix> =
	'::' id <qualified-suffix-opt>;

