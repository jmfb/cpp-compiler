#include "TerminalIdentifierType.h"
#include <stdexcept>

std::string ToString(TerminalIdentifierType type)
{
	switch(type)
	{
	case TerminalIdentifierType::Any:
		return "Any";
	case TerminalIdentifierType::Define:
		return "Define";
	case TerminalIdentifierType::Lookup:
		return "Lookup";
	case TerminalIdentifierType::Match:
		return "Match";
	}
	throw std::logic_error("Invalid terminal identifier type.");
}

