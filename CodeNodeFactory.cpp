#include "CodeNodeFactory.h"
#include <cstdlib>
#include <sstream>

CodeNodePtr CodeNodeFactory::Create(
	const std::string& nonTerminal,
	const std::string& production,
	const std::deque<SentenceItemPtr>& items)
{
	auto productionMap = GetCreators().find(nonTerminal);
	if (productionMap == GetCreators().end())
	{
		std::cout << "Unrecognized nonTerminal: " << nonTerminal << std::endl;
		::exit(0);
	}
	auto creator = productionMap->second.find(production);
	if (creator == productionMap->second.end())
	{
		std::cout << "Unrecognized " << nonTerminal << " production: " << production << std::endl;
		::exit(0);
	}
	return CodeNodePtr { creator->second(items) };
}

std::string CodeNodeFactory::GenerateGrammar()
{
	std::ostringstream out;
	for (auto& nonTerminal: GetCreators())
	{
		out << "<" << nonTerminal.first << "> =" << std::endl;
		for (auto production = begin(nonTerminal.second); production != end(nonTerminal.second); ++production)
		{
			if (production != begin(nonTerminal.second))
				out << " |" << std::endl;
			out << "\t" << production->first;
		}
		out << ";" << std::endl << std::endl;
	}
	return out.str();
}

void CodeNodeFactory::RegisterCreators(NonTerminalEntry entry)
{
	GetCreators().insert(entry);
}

CodeNodeFactory::NonTerminalMap& CodeNodeFactory::GetCreators()
{
	static NonTerminalMap creators;
	return creators;
}

