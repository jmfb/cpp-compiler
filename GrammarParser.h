#pragma once
#include <cstddef>
#include <string>
#include <map>
#include "NonTerminal.h"
#include "Production.h"

class GrammarParser
{
public:
	GrammarParser() = delete;
	GrammarParser(std::map<std::string, NonTerminal>& items);
	GrammarParser(const GrammarParser& rhs) = delete;
	~GrammarParser() = default;

	GrammarParser& operator=(const GrammarParser& rhs) = delete;

	void Parse(const std::string& value);

private:
	void ProcessCharacter(char ch);
	void ProcessEndOfFile();

	enum class State
	{
		Initial,
		LeftSide,
		Middle,
		RightSide,
		NonTerminal,
		Operator,
		Keyword,
		Identifier,
		Constant,
		Comment,
		FirstComment,
		BlockComment,
		BlockCommentEnd
	};

	void OnInitial(char ch);
	void OnLeftSide(char ch);
	void OnMiddle(char ch);
	void OnRightSide(char ch);
	void OnNonTerminal(char ch);
	void OnOperator(char ch);
	void OnKeyword(char ch);
	void OnIdentifier(char ch);
	void OnConstant(char ch);
	void OnComment(char ch);
	void OnFirstComment(char ch);
	void OnBlockComment(char ch);
	void OnBlockCommentEnd(char ch);

	void AddNonTerminalToProduction();

private:
	std::map<std::string, NonTerminal>& items;
	std::size_t line = 1;
	std::size_t column = 1;
	State state = State::Initial;
	State returnToState = State::Initial;
	std::string name;
	NonTerminal nonTerminal;
	Production production;
};

// Functions and members for parsing a grammar from a text file.  The following grammar syntax is supported:
//     grammar = non-terminal-seq-opt;
//     non-terminal-seq = non-terminal non-terminal-seq-opt;
//     non-terminal = non-terminal-name '=' production-list ';';
//     non-terminal-name = '&lt;' {any characters but &gt;} '&gt';
//     production-list = production | production-list '|' production;
//     production = production-item-seq | '@';
//     production-item-seq = production-item production-item-seq-opt;
//     production-item = "id" | "val" | keyword | operator | non-terminal-name;
//     keyword = '"' {any characters but "} '"';
//     operator = '\'' {any characters but '} '\'';
//     single-line-comment = '#' {any characters but \n} '\n';
//     block-comment = '#' '#' {any characters but ##} '#' '#';
//
// Additional non-terminal name parsing logic:
//     Prefix of "id-" indicates an identifier instead of a non-terminal.  Only useful for "id-opt" syntax.
//     Prefix of "op-{any characters but -}-" refers to an operator instead of a non-terminal.
//     Prefix of "key-{any characters but -}-" refers to a keyword instead of a non-terminal.
//     Prefix of "id*" will match on any identifier.  Is also useful with grammar actions as opposed to "id-".
//     Prefix of "id=type" will match on any identifier and will define it as the given type.
//     Prefix of "id:type" will match on any identifier of given type.
//     Prefix of "id.value" will match on an identifier of the given value only.
//     Suffix of "-opt" indicates non-terminal item is optional.  Will automatically generate
//     the non-terminal "name-opt = name | @;".
//     Suffix of ",$name" is only valid for special identifier prefixes and causes symbol table
//     token variable assignment of the given identifier to $name variable useful for later calls
//     to grammar action functions (note that names are not scoped and are reused without error).
//     Grammar action functions follow special prefixes or regular non-terminal names and precede
//     any special suffixes and are of the form "-@action[(arg1[,arg2])]".  The following grammar
//     actions are supported:
//         @push(unnamed), @push(null), @push(block), @push($name)
//         @pop
//         @scope(global), @scope($name)
//         @usens($name)
//         @usesym($name)
//         @alias($alias,$original)
//         @define(type,$name)
//         @defpush(type,$name)
//
// Example Grammar Definition (demonstrating all supported language features with comments)
// ========================================================================================
// #This is just a commented line that goes until the newline.
// <translation-unit> =         #Non-terminal definitions read until the semicolon, not newline.
//      <declaration-seq-opt>;  #Semicolon indicates the end of this non-terminal
// #Note: "<declaration-seq-opt> = <declaration-seq> | @;" is automatically generated.
// <declaration-seq> =      #This non-terminal defined 2 productions.
//      <declaration>       #It is good practice to always put productions on their own line.
//      | <declaration-seq> <declaration>;  #Second production starts after '|' operator.
// <declaration> = <class-declaration> | <variable-declaration>;
// <class-declaration> =
//      <key-partial-opt>       #Partial class keyword is optional
//      "class"                 #Class keyword required
//      <id*,$class-name>       #Matches any identifier and stores token in $class-name variable.
//      #Operator '{' which will invoke @defpush grammar action with type=class-name and
//      #name=$class-name (variable identifying token from previous <id*> terminal.
//      <op-{-@defpush(class-name,$class-name)>
//      <declaration-seq-opt>   #The declarations declared here will be declared in the
//                              #scope of the class we've just declared.
//      <op-}-@pop>             #Pops the scope pushed during @defpush grammar action.
//      ';'                     #class declaration is terminated by semicolon operator.
// <variable-declaration> = <type> <id*,$var> ';';  #We can have simple declaration not use symbol table.
// <type> = <simple-type> | <class-type>;
// <simple-type> = "int"
// ## Commented out this entire block in order to simplify and demonstrate block comment
//      | "bool"
//      | "float"
// ## ; #Block comment immediately ends at matched ## so the previous semicolon is "in".
// #Example of supported qualified types using @scope grammar action.  Note that LR
// #grammars require qualified types to be of the optional suffix type and not the
// #optional prefix type...
// <class-type> = <global-scope-opt> <id:class-name,$1> <qualified-type-opt>;
// <global-scope> = <op-::-@scope(global)>;
// #See that the following qualifies the scope using the previous $1 value and then
// #proceeds to redefine the $1 variable which is what allows the recurring suffix
// #definition to work while maintaining proper qualification via the symbol table.
// <qualified-type> = <op-::-@scope($1)> <id:class-name,$1> <qualified-type-opt>;

