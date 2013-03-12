#include "Token.h"
#include <sstream>

Token::Token(
	TokenType type,
	const Position& position,
	const std::string& value)
	: type(type), position(position), value(value)
{
}

bool Token::IsEndOfFile() const
{
	return type == TokenType::EndOfFile;
}

bool Token::MatchValue(const std::string& value) const
{
	return this->value == value;
}

std::string Token::ToString() const
{
	std::ostringstream out;
	out << position.ToString() << ": <" << ::ToString(type) << "> " << value;
	return out.str();
}

