#pragma once
#include <deque>
#include "SentenceItem.h"
#include <functional>
#include "CodeNode.h"
#include <string>
#include <map>

class CodeNodeFactory
{
public:
	typedef const std::deque<SentenceItemPtr>& Items;
	typedef std::function<CodeNode*(Items)> Creator;
	typedef std::map<std::string, Creator> ProductionMap;
	typedef ProductionMap::value_type ProductionEntry;
	typedef std::map<std::string, ProductionMap> NonTerminalMap;
	typedef NonTerminalMap::value_type NonTerminalEntry;

	static CodeNodePtr Create(
		const std::string& nonTerminal,
		const std::string& production,
		const std::deque<SentenceItemPtr>& items);

	static std::string GenerateGrammar();

	static void RegisterCreators(NonTerminalEntry entry);

private:
	static NonTerminalMap& GetCreators();
};

typedef CodeNodeFactory::Items CodeNodeItems;

