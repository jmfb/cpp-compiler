#pragma once
#include "CharStream.h"

class CharStreamCpp : public CharStream
{
public:
	CharStreamCpp() = delete;
	CharStreamCpp(std::istream& in, const std::string& name);
	CharStreamCpp(const CharStreamCpp& rhs) = delete;
	~CharStreamCpp() = default;

	CharStreamCpp& operator=(const CharStreamCpp& rhs) = delete;

	Char InternalGetNext() final;
	Char ReadNextChar();
};

