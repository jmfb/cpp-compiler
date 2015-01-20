<translation-unit> =  <A> | <B>;
<A> = <C> id ';';
<C> = <E>;
<B> = <D> ';';
<D> = <E> | val;
<E> = id;

##

<translation-unit> =
	<declaration-list-opt>;
<declaration-list> =
	<declaration> <declaration-list-opt>;
<declaration-list-opt> =
	<declaration-list> |
	@;
<declaration> =
	<declaration-statement> |
	<expression-statement>;
<declaration-statement> =
	<type-name> id ';';
<expression-statement> =
	<primary-expression>;
<primary-expression> =
	<qualified-name> |
	val;
<type-name> =
	<qualified-name>;
<qualified-name> =
	id;

##

##

<access-modifier> =
	"public" |
	"protected" |
	"private";

<base-class> =
	<modifier> <qualified-name>;

<base-class-list> =
	<base-class> |
	<base-class> ',' <base-class-list>;

<class-declaration> =
	"class" <qualified-name> '{' <class-member-list-opt> '}' |
	"class" <qualified-name> ':' <base-class-list> '{' <class-member-list-opt> '}';

<class-member> =
	<property>;

<class-member-list> =
	<class-member> <class-member-list-opt>;

<class-member-list-opt> =
	<class-member-list> |
	@;

<function-body> =
	'{' <statement-list-opt> '}' |
	';';

<modifier> =
	<access-modifier> |
	<access-modifier> "static" |
	<access-modifier> "abstract" |
	<access-modifier> "virtual" |
	<access-modifier> "override" |
	<access-modifier> "final";

<property> =
	<modifier> <type-name> <qualified-name> '=' <expression> <property-body> |
	<modifier> <type-name> <qualified-name> <property-body>;

<property-body> =
	'{' <property-function-definition-list> '}' |
	';';

<property-function> =
	"get" |
	"getcopy" |
	"getref" |
	"set" |
	"setcopy";

<property-function-definition> =
	<property-function> <function-body> |
	<access-modifier> <property-function> <function-body>;

<property-function-definition-list> =
	<property-function-definition> |
	<property-function-definition> <property-function-definition-list>;

<expression> =
	<primary-expression>;

<primary-expression> =
	<qualified-name> |
	val;

<qualified-name> =
	id <qualified-suffix-opt>;

<qualified-suffix> =
	'::' id <qualified-suffix-opt>;

<qualified-suffix-opt> =
	<qualified-suffix> |
	@;

<declaration-statement> =
	<type-name> id ';';

<expression-statement> =
	<expression> ';';

<return-statement> =
	"return" ';' |
	"return" <expression> ';';

<statement> =
	<declaration-statement> |
	<expression-statement> |
	<return-statement>;

<statement-list> =
	<statement> <statement-list-opt>;

<statement-list-opt> =
	<statement-list> |
	@;

<declaration> =
	<namespace-declaration> |
	<class-declaration>;

<declaration-list> =
	<declaration> <declaration-list-opt>;

<declaration-list-opt> =
	<declaration-list> |
	@;

<namespace-declaration> =
	"namespace" <qualified-name> '{' <declaration-list-opt> '}';

<translation-unit> =
	<using-list-opt> <declaration-list-opt>;

<using-alias> =
	"using" id '=' <qualified-name> ';';

<using-import> =
	"using" <qualified-name> ';';

<using-item> =
	<using-import> |
	<using-alias>;

<using-list> =
	<using-item> <using-list-opt>;

<using-list-opt> =
	<using-list> |
	@;

<built-in-type-name> =
	"void" |
	"var" |
	"bool" |
	"char" |
	"wchar" |
	"byte" |
	"short" |
	"ushort" |
	"int" |
	"uint" |
	"long" |
	"ulong" |
	"float" |
	"double" |
	"ldouble";

<core-type-name> =
	<built-in-type-name> |
	<qualified-name>;

<pointer-operator> =
	'&' |
	'*' |
	'?' |
	'^' |
	'[' <expression> ']' |
	'[' ']';

<pointer-specifier> =
	<pointer-operator> |
	<pointer-operator> "const";

<pointer-specifier-list> =
	<pointer-specifier> <pointer-specifier-list-opt>;

<pointer-specifier-list-opt> =
	<pointer-specifier-list> |
	@;

<type-name> =
	<core-type-name> <pointer-specifier-list-opt> |
	"const" <core-type-name> <pointer-specifier-list-opt>;


##
##

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


##
