#include "CodeNodeRegister.h"

CodeNodeRegister::CodeNodeRegister(
	const std::string& nonTerminal,
	CodeNodeFactory::ProductionMap productions)
{
	CodeNodeFactory::RegisterCreators({ nonTerminal, productions });
}

