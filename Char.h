#pragma once
#include "Position.h"

class Char
{
public:
	Char() = default;
	Char(const Position& position, char value);
	Char(const Char& rhs) = default;
	~Char() = default;

	Char& operator=(const Char& rhs) = default;

public:
	Position position;
	char value = 0;
};

