#pragma once
#include "CodeNodeFactory.h"

class CodeNodeRegister
{
public:
	CodeNodeRegister(
		const std::string& nonTerminal,
		CodeNodeFactory::ProductionMap productions);
};

