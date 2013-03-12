#pragma once
#include <string>

enum class TokenType
{
	Keyword,
	Identifier,
	Operator,
	Constant,
	EndOfFile
};

std::string ToString(TokenType type);

