#include "CharStreamCpp.h"

CharStreamCpp::CharStreamCpp(std::istream& in, const std::string& name)
	: CharStream(in, name)
{
}

Char CharStreamCpp::InternalGetNext()
{
	auto result = ReadNextChar();
	if (result.value == '\\')
	{
		auto next = ReadNextChar();
		if (next.value == '\n')
			return InternalGetNext();
		else
			PutBack(next);
	}
	return result;
}

Char CharStreamCpp::ReadNextChar()
{
	Char result { { name, line, column }, 0 };
	if (in.get(result.value))
	{
		++column;
		if (result.value == '\n')
		{
			++line;
			column = 1;
		}
	}
	return result;
}

