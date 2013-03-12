#include "Item1.h"
#include <sstream>

Item1::Item1(
	const std::string& nonTerminal,
	const Production& production,
	std::size_t index,
	const Terminal& follow)
	: nonTerminal(nonTerminal), production(production), index(index), follow(follow)
{
}

bool Item1::HasNonTerminalSuffix(std::string& suffix) const
{
	// A -> a.Bb
	if (index >= production.items.size() || !production.items[index].isNonTerminal)
		return false;
	suffix = production.items[index].nonTerminal;
	return true;
}

bool Item1::IsNext(const ProductionItem& item) const
{
	return index < production.items.size() && production.items[index] == item;
}

ProductionItem Item1::GetNext() const
{
	if (index < production.items.size())
		return production.items[index];
	return {};
}

std::string Item1::ToString() const
{
	std::ostringstream out;
	out << nonTerminal << " = ";
	for (auto index = 0ul; index < production.items.size(); ++index)
	{
		if (index == this->index)
			out << ". ";
		out << production.items[index].ToString() << " ";
	}
	if (index >= production.items.size())
		out << ". ";
	out << ", " << follow.ToString();
	return out.str();
}

bool Item1::operator==(const Item1& rhs) const
{
	return nonTerminal == rhs.nonTerminal &&
		production == rhs.production &&
		index == rhs.index &&
		follow == rhs.follow;
}

bool Item1::operator!=(const Item1& rhs) const
{
	return !operator==(rhs);
}

