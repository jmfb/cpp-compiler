#pragma once
#include <vector>
#include "Macro.h"
#include "MacroType.h"
#include "PreProcessorToken.h"
#include "MacroTableExpander.h"

class MacroTable : public MacroTableExpander
{
public:
	MacroTable() = default;
	MacroTable(const MacroTable& rhs) = default;
	~MacroTable() = default;

	MacroTable& operator=(const MacroTable& rhs) = default;

	void Define(const std::vector<PreProcessorToken>& tokens);
	void Undefine(const std::vector<PreProcessorToken>& tokens);
	MacroType GetMacroType(const std::string& value) const final;
	std::vector<PreProcessorToken> Expand(
		const PreProcessorToken& token,
		const std::vector<std::vector<PreProcessorToken>>& parameters,
		std::set<std::string> visited) const final;

public:
	std::vector<Macro> macros;
};

