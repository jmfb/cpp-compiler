#pragma once
#include <string>
#include <vector>
#include <utility>
#include "Production.h"
#include "Terminal.h"

class NonTerminal
{
public:
	NonTerminal() = default;
	NonTerminal(const std::string& name);
	NonTerminal(const NonTerminal& rhs) = default;
	~NonTerminal() = default;

	NonTerminal& operator=(const NonTerminal& rhs) = default;

	NonTerminal& AddItem(const Production& production);

	std::pair<bool, NonTerminal> EliminateLeftRecursion(const std::string& name);
	std::pair<bool, NonTerminal> LeftFactor(const std::string& name);

	void AddFirst(const Terminal& terminal);
	void MergeFirst(const std::vector<Terminal>& terminals);
	void AddFollow(const Terminal& terminal);
	void MergeFollow(const std::vector<Terminal>& terminals);
	void AddFirstToFollow(const std::vector<Terminal>& terminals);

public:
	std::string name;
	std::vector<Production> items;
	std::vector<Terminal> first;
	std::vector<Terminal> follow;
};

