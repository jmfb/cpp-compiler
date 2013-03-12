#pragma once
#include <vector>
#include <string>
#include <set>
#include "PreProcessorToken.h"
#include "MacroType.h"

class MacroTableExpander
{
public:
	virtual MacroType GetMacroType(const std::string& value) const = 0;
	virtual std::vector<PreProcessorToken> Expand(
		const PreProcessorToken& token,
		const std::vector<std::vector<PreProcessorToken>>& parameters,
		std::set<std::string> visited) const = 0;
};

