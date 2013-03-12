#include "Grammar.h"
#include <algorithm>
#include <stdexcept>
#include "GrammarParser.h"

Grammar& Grammar::AddItem(const NonTerminal& nonTerminal)
{
	items.insert({ nonTerminal.name, nonTerminal });
	return *this;
}

void Grammar::Augment(const std::string& start, const std::string& augmentedStart)
{
	AddItem(
		NonTerminal { augmentedStart }.AddItem(
			Production {}.AddItem(start)));
}

void Grammar::Simplify()
{
	EliminateLeftRecursion();
	LeftFactor();
}

void Grammar::CalculateFirst()
{
	for (auto& item: items)
		if (item.second.first.empty())
			CalculateFirst(item.second, 1);
}

void Grammar::CalculateFollow(const std::string& start)
{
	//Add $ (input right end marker) to start symbol
	auto iter = items.find(start);
	if (iter == items.end())
		throw std::logic_error("Start symbol <" + start + "> was not found in grammar.");
	iter->second.follow.push_back(Terminal::Empty);

	//If exists A->aBb, then FOLLOW(B) += FIRST(b) - @
	std::vector<std::pair<std::string, std::string>> merge;
	for (auto& nt: items)
	{
		for (auto& production: nt.second.items)
		{
			if (production.items.empty())
				continue;

			for (auto index = 0ul; index < production.items.size(); ++index)
			{
				auto& item = production.items[index];
				if (item.isNonTerminal)
				{
					auto ntIter = items.find(item.nonTerminal);
					if (ntIter == items.end())
						throw std::logic_error("Undefined non-terminal: " + item.nonTerminal);
					auto& current = ntIter->second;
					auto countEmpty = 0ul;
					for (auto next = index + 1; next < production.items.size(); ++next)
					{
						auto& after = production.items[next];
						if (after.isTerminal)
						{
							current.AddFollow(after.terminal);
							break;
						}
						else
						{
							auto afterIter = items.find(after.nonTerminal);
							if (afterIter == items.end())
								throw std::logic_error("Undefined non-terminal: " + after.nonTerminal);
							auto& lookup = afterIter->second;
							current.AddFirstToFollow(lookup.first);
							auto emptyIter = std::find(lookup.first.begin(), lookup.first.end(), Terminal::Empty);
							if (emptyIter != lookup.first.end())
								++countEmpty;
							else
								break;
						}
					}
					if (countEmpty == (production.items.size() - index - 1))
						merge.push_back({ current.name, nt.second.name });
				}
			}
		}
	}

	//If exists A->aB or A->aBb where FIRST(b) has @, then FOLLOW(B) += FOLLOW(A)
	while (!merge.empty())
		ResolveFollow(merge[0].first, merge);
}

void Grammar::Parse(const std::string& grammar)
{
	GrammarParser parser { items };
	parser.Parse(grammar);
}

void Grammar::EliminateLeftRecursion()
{
	std::vector<NonTerminal> factors;
	for (auto& item: items)
	{
		auto name = NextValidNonTerminalName(item.second.name, "'");
		auto factor = item.second.EliminateLeftRecursion(name);
		if (factor.first)
			factors.push_back(factor.second);
	}
	for (auto& nonTerminal: factors)
		AddItem(nonTerminal);
}

void Grammar::LeftFactor()
{
	std::vector<NonTerminal> factors;
	std::vector<std::string> names;

	for (auto& item: items)
	{
		for (;;)
		{
			auto name = NextValidNonTerminalName(item.second.name, "*", names);
			auto factor = item.second.LeftFactor(name);
			if (!factor.first)
				break;
			names.push_back(name);
			factors.push_back(factor.second);
		}
	}

	for (auto index = 0ul; index < factors.size(); ++index)
	{
		auto nonTerminal = factors[index];
		for (;;)
		{
			auto name = NextValidNonTerminalName(nonTerminal.name, "*", names);
			auto factor = nonTerminal.LeftFactor(name);
			if (!factor.first)
				break;
			names.push_back(name);
			factors.push_back(factor.second);
		}
		AddItem(nonTerminal);
	}
}

std::string Grammar::NextValidNonTerminalName(const std::string& name, const std::string& suffix) const
{
	return NextValidNonTerminalName(name, suffix, {});
}

std::string Grammar::NextValidNonTerminalName(
	const std::string& name,
	const std::string& suffix,
	const std::vector<std::string>& names) const
{
	for (auto newName = name + suffix; ; newName += suffix)
	{
		auto itemIter = items.find(newName);
		if (itemIter != items.end())
			continue;
		auto nameIter = std::find(names.begin(), names.end(), newName);
		if (nameIter != names.end())
			continue;
		return newName;
	}
}

void Grammar::CalculateFirst(NonTerminal& nonTerminal, long depth)
{
	if (depth == 256)
		throw std::logic_error("Recursive non-terminal detected: " + nonTerminal.name);

	for (auto& production: nonTerminal.items)
	{
		if (production.items.empty())
			nonTerminal.AddFirst(Terminal::Empty);
		else if (production.items[0].isTerminal)
		{
			production.AddFirst(production.items[0].terminal);
			nonTerminal.AddFirst(production.items[0].terminal);
		}
		else
		{
			auto countEmpty = 0ul;
			for (auto& item: production.items)
			{
				if (item.isTerminal)
				{
					production.AddFirst(item.terminal);
					nonTerminal.AddFirst(item.terminal);
					break;
				}
				else
				{
					auto iter = items.find(item.nonTerminal);
					if (iter == items.end())
						throw std::logic_error("Undefined non-literal <" + item.nonTerminal +
							"> references from <" + nonTerminal.name + ">.");
					auto& lookup = iter->second;
					if (lookup.first.empty())
						CalculateFirst(lookup, depth + 1);
					production.MergeFirst(lookup.first);
					nonTerminal.MergeFirst(lookup.first);
					auto iterEmpty = std::find(lookup.first.begin(), lookup.first.end(), Terminal::Empty);
					if (iterEmpty != lookup.first.end())
						++countEmpty;
					else
						break;
				}
			}
			if (countEmpty == production.items.size())
			{
				production.AddFirst(Terminal::Empty);
				nonTerminal.AddFirst(Terminal::Empty);
			}
		}
	}
}

void Grammar::ResolveFollow(
	std::string name,
	std::vector<std::pair<std::string, std::string>>& merge)
{
	std::vector<std::string> sources;
	for (auto index = 0ul; index < merge.size(); )
	{
		auto& item = merge[index];
		if (item.first == name && std::find(sources.begin(), sources.end(), item.second) == sources.end())
		{
			sources.push_back(item.second);
			merge.erase(merge.begin() + index);
		}
		else
			++index;
	}

	for (auto index = 0ul; index < sources.size(); ++index)
	{
		for (auto& item: merge)
		{
			if (item.first == sources[index] && item.second != name &&
				std::find(sources.begin(), sources.end(), item.second) == sources.end())
				sources.push_back(item.second);
		}
	}
	for (auto& source: sources)
		items.find(name)->second.MergeFollow(items.find(source)->second.follow);
}

