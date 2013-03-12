////////////////////////////////////////////////////////////////////////////////
// Filename:    TokenStreamCpp.cpp
// Description: This file implements all TokenStreamCpp member functions.
//
// Created:     2013-02-14 22:52:00
// Author:      Jacob Buysse
////////////////////////////////////////////////////////////////////////////////
#include "TokenStreamCpp.h"
#include <set>
#include <stdexcept>

TokenStreamCpp::TokenStreamCpp(PreProcessor& processor)
	: TokenStream(processor)
{
}

Token TokenStreamCpp::CreateToken(const PreProcessorToken& ppToken) const
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

bool TokenStreamCpp::IsKeyword(const std::string& value)
{
	static const std::set<std::string> validKeywords {
		"alignas",
		"alignof",
		"asm",
		"auto",
		"bool",
		"break",
		"case",
		"catch",
		"char",
		"char16_t",
		"char32_t",
		"class",
		"const",
		"constexpr",
		"const_cast",
		"continue",
		"decltype",
		"default",
		"delete",
		"do",
		"double",
		"dynamic_cast",
		"else",
		"enum",
		"explicit",
		"export",
		"extern",
		"false",
		"float",
		"for",
		"friend",
		"goto",
		"if",
		"inline",
		"int",
		"long",
		"mutable",
		"namespace",
		"new",
		"noexcept",
		"nullptr",
		"operator",
		"private",
		"protected",
		"public",
		"register",
		"reinterpret_cast",
		"return",
		"short",
		"signed",
		"sizeof",
		"static",
		"static_assert",
		"static_cast",
		"struct",
		"switch",
		"template",
		"this",
		"thread_local",
		"throw",
		"true",
		"try",
		"typedef",
		"typeid",
		"typename",
		"union",
		"unsigned",
		"using",
		"virtual",
		"void",
		"volatile",
		"wchar_t",
		"while"
	};
	return validKeywords.find(value) != validKeywords.end();
}

bool TokenStreamCpp::IsValidOperator(const std::string& value)
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
		"?", ":", "::", ";",
		"{", "}", "(", ")", "[", "]"
	};
	return validOperators.find(value) != validOperators.end();
}

