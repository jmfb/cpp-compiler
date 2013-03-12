#include "NonTerminal.h"
#include <algorithm>
#include <stdexcept>

NonTerminal::NonTerminal(const std::string& name)
	: name(name)
{
}

NonTerminal& NonTerminal::AddItem(const Production& production)
{
	items.push_back(production);
	return *this;
}

std::pair<bool, NonTerminal> NonTerminal::EliminateLeftRecursion(const std::string& name)
{
	NonTerminal factor { name };
	std::vector<Production> match;
	std::vector<Production> rest;
	for (auto& production: items)
	{
		if (!production.items.empty() &&
			!production.items[0].isTerminal &&
			production.items[0].nonTerminal == this->name)
			match.push_back(production);
		else
			rest.push_back(production);
	}
	if (match.empty())
		return { false, factor };
	if (rest.empty())
		throw std::logic_error("Invalid production (A->Aa)");

	//A->Aa|B to A->BA', A'->aA'|@
	items.clear();
	for (auto& production: rest)
		items.push_back(production.AddItem(name));
	for (auto& production: match)
		factor.AddItem(production.RemoveFront().AddItem(name));
	factor.AddItem({});
	return { true, factor };
}

std::pair<bool, NonTerminal> NonTerminal::LeftFactor(const std::string& name)
{
	NonTerminal factor { name };
	//A->aB1|aB2 to A->aA', A'->B1|B2
	for (auto index = 0ul; index < items.size(); ++index)
	{
		auto& production = items[index];
		if (production.items.empty())
			continue;
		auto item = production.items[0];
		std::vector<decltype(index)> match;
		for (auto next = index + 1; next < items.size(); ++next)
			if (!items[next].items.empty() && items[next].items[0] == item)
				match.push_back(next);
		if (!match.empty())
		{
			factor.AddItem(production.RemoveFront());
			for (auto indexToMove: match)
				factor.AddItem(items[indexToMove].RemoveFront());
			for (auto iter = match.rbegin(); iter != match.rend(); ++iter)
				items.erase(items.begin() + *iter);
			items[index] = Production{}.AddItem(item).AddItem(name);
			return { true, factor};
		}
	}
	return { false, factor };
}

void NonTerminal::AddFirst(const Terminal& terminal)
{
	auto iter = std::find(first.begin(), first.end(), terminal);
	if (iter == first.end())
		first.push_back(terminal);
}

void NonTerminal::MergeFirst(const std::vector<Terminal>& terminals)
{
	for (auto& terminal: terminals)
		if (terminal != Terminal::Empty)
			AddFirst(terminal);
}

void NonTerminal::AddFollow(const Terminal& terminal)
{
	auto iter = std::find(follow.begin(), follow.end(), terminal);
	if (iter == follow.end())
		follow.push_back(terminal);
}

void NonTerminal::MergeFollow(const std::vector<Terminal>& terminals)
{
	for (auto& terminal: terminals)
		AddFollow(terminal);
}

void NonTerminal::AddFirstToFollow(const std::vector<Terminal>& terminals)
{
	for (auto& terminal: terminals)
		if (terminal != Terminal::Empty)
			AddFollow(terminal);
}

