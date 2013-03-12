#include "TokenType.h"
#include <stdexcept>

std::string ToString(TokenType type)
{
	switch(type)
	{
	case TokenType::Keyword:
		return "Keyword";
	case TokenType::Identifier:
		return "Identifier";
	case TokenType::Operator:
		return "Operator";
	case TokenType::Constant:
		return "Constant";
	case TokenType::EndOfFile:
		return "EndOfFile";
	}
	throw std::logic_error("Invalid token type.");
}

