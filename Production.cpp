#include "Production.h"
#include <algorithm>
#include <sstream>
#include <stdexcept>

Production& Production::AddItem(const Terminal& terminal)
{
	items.push_back({ terminal });
	return *this;
}

Production& Production::AddItem(const std::string& nonTerminal)
{
	items.push_back({ nonTerminal });
	return *this;
}

Production& Production::AddItem(const ProductionItem& item)
{
	items.push_back(item);
	return *this;
}

Production& Production::RemoveFront()
{
	if (items.empty())
		throw std::logic_error("Cannot remove the front of an empty production.");
	items.erase(items.begin());
	return *this;
}

void Production::AddFirst(const Terminal& terminal)
{
	auto iter = std::find(first.begin(), first.end(), terminal);
	if (iter == first.end())
		first.push_back(terminal);
}

void Production::MergeFirst(const std::vector<Terminal>& terminals)
{
	for (auto& terminal: terminals)
		AddFirst(terminal);
}

std::string Production::ToString() const
{
	if (items.empty())
		return "@";
	std::ostringstream out;
	auto iter = begin(items);
	out << iter->ToString();
	for (++iter; iter != end(items); ++iter)
		out << " " << iter->ToString();
	return out.str();
}

bool Production::operator==(const Production& rhs) const
{
	return items == rhs.items;
}

bool Production::operator!=(const Production& rhs) const
{
	return !operator==(rhs);
}

