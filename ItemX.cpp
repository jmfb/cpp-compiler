#include "ItemX.h"
#include <sstream>
#include <algorithm>

ItemX::ItemX(
	const std::string& nonTerminal,
	const Production& production,
	std::size_t index)
	: nonTerminal(nonTerminal), production(production), index(index)
{
}

bool ItemX::HasNonTerminalSuffix(std::string& suffix) const
{
	// A -> a.Bb
	if (index >= production.items.size() || !production.items[index].isNonTerminal)
		return false;
	suffix = production.items[index].nonTerminal;
	return true;
}

bool ItemX::IsNext(const ProductionItem& item) const
{
	return index < production.items.size() && production.items[index] == item;
}

ProductionItem ItemX::GetNext() const
{
	if (index < production.items.size())
		return production.items[index];
	return {};
}

bool ItemX::AddLookAhead(const Terminal& terminal)
{
	auto iter = std::find(lookAhead.begin(), lookAhead.end(), terminal);
	if (iter != lookAhead.end())
		return false;
	lookAhead.push_back(terminal);
	return true;
}

void ItemX::AddPropogate(std::size_t state, std::size_t subItem)
{
	ItemXPropogate prop { state, subItem };
	auto iter = std::find(propogate.begin(), propogate.end(), prop);
	if (iter == propogate.end())
		propogate.push_back(prop);
}

std::string ItemX::ToString() const
{
	std::ostringstream out;
	out << "[" << nonTerminal << " -> ";
	for (auto index = 0ul; index < production.items.size(); ++index)
	{
		if (index == this->index)
			out << ". ";
		out << production.items[index].ToString();
	}
	if (this->index >= production.items.size())
		out << ". ";
	out << ", ";
	for (auto& terminal: lookAhead)
		out << terminal.ToString() << "/";
	out << "] Prop";
	for (auto& prop: propogate)
		out << "," << prop.ToString();
	return out.str();
}

bool ItemX::operator==(const ItemX& rhs) const
{
	return nonTerminal == rhs.nonTerminal &&
		production == rhs.production &&
		index == rhs.index;
}

bool ItemX::operator!=(const ItemX& rhs) const
{
	return !operator==(rhs);
}

