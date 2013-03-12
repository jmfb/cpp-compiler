#include "MacroTable.h"
#include <stdexcept>
#include <algorithm>
#include "MacroExpander.h"

void MacroTable::Define(const std::vector<PreProcessorToken>& tokens)
{
	Macro macro { tokens };
	auto iter = std::find_if(macros.begin(), macros.end(), [&](const Macro& rhs)
	{
		return rhs.name == macro.name;
	});
	if (iter == macros.end())
		macros.push_back(macro);
	else if (!iter->IsIdentical(macro))
		throw std::logic_error("Multiple different definitions of macro.");
}

void MacroTable::Undefine(const std::vector<PreProcessorToken>& tokens)
{
	for (auto& token: tokens)
	{
		if (token.CanIgnore())
			continue;
		if (token.type != PreProcessorTokenType::Identifier)
			throw std::logic_error("You can only undefine identifiers.");

		auto iter = std::find_if(macros.begin(), macros.end(), [&](const Macro& macro)
		{
			return macro.name == token.value;
		});
		if (iter != macros.end())
			macros.erase(iter);
		break;
	}
}

MacroType MacroTable::GetMacroType(const std::string& value) const
{
	auto iter = std::find_if(macros.begin(), macros.end(), [&](const Macro& macro)
	{
		return macro.name == value;
	});
	if (iter == macros.end())
		return MacroType::Undefined;
	return iter->isFunction ? MacroType::Function : MacroType::Object;
}

std::vector<PreProcessorToken> MacroTable::Expand(
	const PreProcessorToken& token,
	const std::vector<std::vector<PreProcessorToken>>& parameters,
	std::set<std::string> visited) const
{
	auto iter = std::find_if(macros.begin(), macros.end(), [&](const Macro& macro)
	{
		return macro.name == token.value;
	});
	if (iter == macros.end())
		throw std::logic_error("Cannot expand an identifier that is not a macro.");
	MacroExpander expander { *iter, token.position, parameters, visited };
	expander.Expand(*this);
	return expander.expansion;
}

