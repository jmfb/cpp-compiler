#pragma once
#include <map>
#include <string>
#include "NonTerminal.h"

class Grammar
{
public:
	Grammar() = default;
	Grammar(const Grammar& rhs) = default;
	~Grammar() = default;

	Grammar& operator=(const Grammar& rhs) = default;

	Grammar& AddItem(const NonTerminal& nonTerminal);
	void Augment(const std::string& start, const std::string& augmentedStart);
	void Simplify();
	void CalculateFirst();
	void CalculateFollow(const std::string& start);

	void Parse(const std::string& grammar);

private:
	void EliminateLeftRecursion();
	void LeftFactor();
	std::string NextValidNonTerminalName(const std::string& name, const std::string& suffix) const;
	std::string NextValidNonTerminalName(
		const std::string& name,
		const std::string& suffix,
		const std::vector<std::string>& names) const;
	void CalculateFirst(NonTerminal& nonTerminal, long depth);
	void ResolveFollow(
		std::string name,
		std::vector<std::pair<std::string, std::string>>& merge);

public:
	std::map<std::string, NonTerminal> items;
};

