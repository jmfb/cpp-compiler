#include "CodeNodeFactory.h"
#include <cstdlib>
#include <sstream>
#include "CodeTypeName.h"
#include "CodeDeclaration.h"
#include "CodeDeclarationList.h"
#include "CodeTranslationUnit.h"

CodeNodeFactory::CodeNodeFactory()
	: creators {
		CodeTypeName::creator,
		CodeDeclaration::creator,
		CodeDeclarationList::creator,
		CodeDeclarationList::optionalCreator,
		CodeTranslationUnit::creator
	}
{
}

CodeNodePtr CodeNodeFactory::Create(
	const std::string& nonTerminal,
	const std::string& production,
	const std::deque<SentenceItemPtr>& items) const
{
	auto productionMap = creators.find(nonTerminal);
	if (productionMap == creators.end())
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
	return creator->second(items);
}

std::string CodeNodeFactory::GenerateGrammar() const
{
	std::ostringstream out;
	for (auto& nonTerminal: creators)
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

