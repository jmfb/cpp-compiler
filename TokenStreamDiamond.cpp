#include "TokenStreamDiamond.h"
#include <stdexcept>
#include <set>

TokenStreamDiamond::TokenStreamDiamond(PreProcessor& processor)
	: TokenStream(processor)
{
}

Token TokenStreamDiamond::CreateToken(const PreProcessorToken& ppToken) const
{
	switch(ppToken.type)
	{
	case PreProcessorTokenType::Identifier:
		if (IsKeyword(ppToken.value))
			return { TokenType::Keyword, ppToken.position, ppToken.value };
		return { TokenType::Identifier, ppToken.position, ppToken.value };

	case PreProcessorTokenType::Operator:
		if (!IsValidOperator(ppToken.value))
			throw std::logic_error("Invalid operator: " + ppToken.value);
		return { TokenType::Operator, ppToken.position, ppToken.value };

	case PreProcessorTokenType::Number:
	case PreProcessorTokenType::String:
	case PreProcessorTokenType::Character:
		return { TokenType::Constant, ppToken.position, ppToken.value };

	default:
		throw std::logic_error("Invalid pp-token: " + ppToken.value);
	}
}

bool TokenStreamDiamond::IsKeyword(const std::string& value)
{
	static const std::set<std::string> validKeywords {
		"using", "namespace",
		"class", "enum",
		"public", "protected", "private",
		"static", "virtual",
		"const", "var",
		"void", "bool",
		"char", "wchar", "byte",
		"short", "ushort", "int", "uint", "long", "ulong",
		"float", "double", "ldouble",
		"null", "true", "false", "this",
		"default", "delete",
		"return",
		"get", "getcopy", "getref", "set", "setcopy"
	};
	return validKeywords.find(value) != validKeywords.end();
}

bool TokenStreamDiamond::IsValidOperator(const std::string& value)
{
	static const std::set<std::string> validOperators {
		"+", "-", "*", "/", "%",
		"++", "--",
		"&", "~", "|", "^",
		">>", "<<",
		"&&", "||", "!",
		"==", "!=", "<", "<=", ">", ">=",
		"=", "+=", "-=", "*=", "/=", "%=",
		"|=", "&=", "^=",
		">>=", "<<=",
		",", ".", "->", ".*", "->*", "...",
		":", "::", ";", "@",
		"<:", ":>",
		"{", "}", "(", ")", "[", "]",
		"?", "??", "?=", "?.", "?.*", "?->", "?->*"
	};
	return validOperators.find(value) != validOperators.end();
}

