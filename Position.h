#pragma once
#include <string>
#include <cstddef>

class Position
{
public:
	Position() = default;
	Position(const std::string& source, std::size_t line, std::size_t column);
	Position(const Position& rhs) = default;
	~Position() = default;

	Position& operator=(const Position& rhs) = default;

	std::string ToString() const;

public:
	std::string source;
	std::size_t line = 0;
	std::size_t column = 0;
};

