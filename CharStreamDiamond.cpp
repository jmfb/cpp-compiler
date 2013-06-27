#include "CharStreamDiamond.h"

CharStreamDiamond::CharStreamDiamond(std::istream& in, const std::string& name)
	: CharStream(in, name)
{
}

Char CharStreamDiamond::InternalGetNext()
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

