#pragma once
#include <string>

enum class TerminalIdentifierType
{
	Any,	//<id*,$name>
	Define,	//<id=type,$name>
	Lookup,	//<id:type,$name>
	Match	//<id.value>
};

std::string ToString(TerminalIdentifierType type);

