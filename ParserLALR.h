#pragma once
#include <string>
#include "TransitionTable.h"
#include "ConflictResolver.h"

class ParserLALR
{
public:
	ParserLALR() = delete;
	ParserLALR(const ParserLALR& rhs) = delete;
	~ParserLALR() = delete;

	ParserLALR& operator=(const ParserLALR& rhs) = delete;

	static TransitionTable Generate(const std::string& grammar, const std::string& start, ConflictResolver& resolver);
};

