#include "PreProcessorTokenType.h"
#include <stdexcept>

std::string ToString(PreProcessorTokenType type)
{
	switch(type)
	{
	case PreProcessorTokenType::Whitespace:
		return "Whitespace";
	case PreProcessorTokenType::Newline:
		return "Newline";
	case PreProcessorTokenType::Comment:
		return "Comment";
	case PreProcessorTokenType::Identifier:
		return "Identifier";
	case PreProcessorTokenType::Number:
		return "Number";
	case PreProcessorTokenType::Character:
		return "Character";
	case PreProcessorTokenType::String:
		return "String";
	case PreProcessorTokenType::Operator:
		return "Operator";
	case PreProcessorTokenType::OtherChar:
		return "OtherChar";
	case PreProcessorTokenType::EndOfFile:
		return "EndOfFile";
	}
	throw std::logic_error("Invalid preprocessor token type.");
}

