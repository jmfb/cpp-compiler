#include "ItemSet1.h"
#include <sstream>
#include <algorithm>
#include <stdexcept>

bool ItemSet1::AddItem(const Item1& item)
{
	auto iter = std::find(items.begin(), items.end(), item);
	if (iter != items.end())
		return false;
	items.push_back(item);
	return true;
}

ItemSet1 ItemSet1::Closure(const Grammar& g) const
{
	ItemSet1 closure;
	closure.items = items;
	for (auto index = 0ul; index < closure.items.size(); ++index)
	{
		auto item = closure.items[index];
		std::string suffix;
		if (item.HasNonTerminalSuffix(suffix))
		{
			std::vector<Terminal> followers;
			if ((item.index + 1) >= item.production.items.size())
				followers.push_back(Terminal::Empty);
			else
			{
				auto emptyCount = 0ul;
				for (auto next = item.index + 1; next < item.production.items.size(); ++next)
				{
					auto& pitem = item.production.items[next];
					if (pitem.isTerminal)
					{
						auto iter = std::find(followers.begin(), followers.end(), pitem.terminal);
						if (iter == followers.end())
							followers.push_back(pitem.terminal);
						break;
					}
					else
					{
						auto foundEmpty = false;
						auto iter = g.items.find(pitem.nonTerminal);
						if (iter == g.items.end())
							throw std::logic_error("WTF");
						for (auto& terminal: iter->second.first)
						{
							if (terminal == Terminal::Empty)
							{
								foundEmpty = true;
								++emptyCount;
							}
							else
							{
								auto iter = std::find(followers.begin(), followers.end(), terminal);
								if (iter == followers.end())
									followers.push_back(terminal);
							}
						}
						if (!foundEmpty)
							break;
					}
				}
				if (emptyCount == (item.production.items.size() - item.index - 1))
					followers.push_back(Terminal::Empty);
			}

			auto iter = g.items.find(suffix);
			if (iter == g.items.end())
				throw std::logic_error("WTF");
			auto& nonTerminal = iter->second;
			for (auto& production: nonTerminal.items)
				for (auto& follow: followers)
					closure.AddItem({ suffix, production, 0, follow == Terminal::Empty ? item.follow : follow });
		}
	}
	return closure;
}

std::vector<ProductionItem> ItemSet1::CalculateUniqueNext() const
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

std::string ItemSet1::ToString() const
{
	std::ostringstream out;
	for (auto& item: items)
		out << "  " << item.ToString();
	return out.str();
}

bool ItemSet1::operator==(const ItemSet1& rhs) const
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

bool ItemSet1::operator!=(const ItemSet1& rhs) const
{
	return !operator==(rhs);
}

