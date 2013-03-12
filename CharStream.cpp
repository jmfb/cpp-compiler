#include "CharStream.h"

CharStream::CharStream(std::istream& in, const std::string& name)
	: in(in), name(name)
{
}

const std::string& CharStream::GetName() const
{
	return name;
}

bool CharStream::IsEOF() const
{
	return !in && buffer.empty();
}

void CharStream::MoveFirst()
{
	in.clear();
	in.seekg(0);
	line = 1;
	column = 1;
	buffer.clear();
}

void CharStream::PutBack(Char ch)
{
	buffer.push_back(ch);
}

Char CharStream::GetNext()
{
	if (!buffer.empty())
	{
		auto result = buffer.back();
		buffer.pop_back();
		return result;
	}

	return InternalGetNext();
}

Char CharStream::ScanPast(const std::string& str)
{
	Char retval;
	std::vector<Char> scanned;
	while (!IsEOF())
	{
		Char ch = GetNext();
		if (ch.value == 0)
			break;
		scanned.push_back(ch);
		if (str.find(ch.value) != std::string::npos)
			continue;
		retval = ch;
		break;
	}
	buffer.insert(buffer.end(), scanned.rbegin(), scanned.rend());
	return retval;
}

