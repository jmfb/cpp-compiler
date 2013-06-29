#pragma once
#include "CodeNodeFactory.h"
#include "Sentence.h"

class CodeNodeRegister
{
public:
	CodeNodeRegister(
		const std::string& nonTerminal,
		CodeNodeFactory::ProductionMap productions);
};

