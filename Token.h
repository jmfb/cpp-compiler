#pragma once
#include <string>
#include "TokenType.h"
#include "Position.h"

class Token
{
public:
	Token() = default;
	Token(
		TokenType type,
		const Position& position,
		const std::string& value);
	Token(const Token& rhs) = default;
	~Token() = default;

	Token& operator=(const Token& rhs) = default;

	bool IsEndOfFile() const;
	bool MatchValue(const std::string& value) const;
	std::string ToString() const;

public:
	TokenType type = TokenType::EndOfFile;
	Position position;
	std::string value;
};

