#pragma once
#include <string>

enum class PreProcessorTokenType
{
	Whitespace,
	Newline,
	Comment,
	Identifier,
	Number,
	Character,
	String,
	Operator,
	OtherChar,
	EndOfFile
};

std::string ToString(PreProcessorTokenType type);

