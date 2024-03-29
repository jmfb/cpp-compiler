#include "ItemSetX.h"
#include <sstream>
#include <algorithm>
#include <stdexcept>

bool ItemSetX::AddItem(const ItemX& item)
{
	auto iter = std::find(items.begin(), items.end(), item);
	if (iter != items.end())
		return false;
	items.push_back(item);
	return true;
}

ItemSetX ItemSetX::Closure(const Grammar& g) const
{
	ItemSetX closure;
	closure.items = items;
	for (auto index = 0ul; index < closure.items.size(); ++index)
	{
		auto& item = closure.items[index];
		std::string suffix;
		if (item.HasNonTerminalSuffix(suffix))
		{
			auto iter = g.items.find(suffix);
			if (iter == g.items.end())
				throw std::logic_error("WTF");
			auto& nonTerminal = iter->second;
			for (auto& production: nonTerminal.items)
				closure.AddItem({ suffix, production, 0 });
		}
	}
	return closure;
}

ItemSetX ItemSetX::Goto(const Grammar& g, const ProductionItem& pitem) const
{
	return GotoRaw(pitem).Closure(g);
}

ItemSetX ItemSetX::GotoRaw(const ProductionItem& pitem) const
{
	ItemSetX retval;
	for (auto& item: items)
		if (item.IsNext(pitem))
			retval.items.push_back({ item.nonTerminal, item.production, item.index + 1 });
	return retval;
}

std::vector<ProductionItem> ItemSetX::CalculateUniqueNext() const
{
	std::vector<ProductionItem> retval;
	for (auto& item: items)
	{
		auto next = item.GetNext();
		if (next.IsNull())
			continue;
		auto iter = std::find(retval.begin(), retval.end(), next);
		if (iter == retval.end())
			retval.push_back(next);
	}
	return retval;
}

std::size_t ItemSetX::FindSubItem(const Production& production, std::size_t dot) const
{
	for (auto index = 0ul; index < items.size(); ++index)
		if (items[index].production == production && items[index].index == dot)
			return index;
	throw std::logic_error("Missing child production...");
}

std::string ItemSetX::ToString() const
{
	std::ostringstream out;
	for (auto& item: items)
		out << "  " << item.ToString();
	return out.str();
}

bool ItemSetX::operator==(const ItemSetX& rhs) const
{
	if (items.size() != rhs.items.size())
		return false;
	for (auto& item: items)
	{
		auto iter = std::find(rhs.items.begin(), rhs.items.end(), item);
		if (iter == rhs.items.end())
			return false;
	}
	return true;
}

bool ItemSetX::operator!=(const ItemSetX& rhs) const
{
	return !operator==(rhs);
}

