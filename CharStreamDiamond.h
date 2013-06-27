#pragma once
#include "CharStream.h"

class CharStreamDiamond : public CharStream
{
public:
	CharStreamDiamond() = delete;
	CharStreamDiamond(std::istream& in, const std::string& name);
	CharStreamDiamond(const CharStreamDiamond& rhs) = delete;
	~CharStreamDiamond() = default;

	CharStreamDiamond& operator=(const CharStreamDiamond& rhs) = delete;

	Char InternalGetNext() final;
};

