#include <fstream>
#include <iostream>
#include <sstream>
#include <exception>
#include "CharStreamCpp.h"
#include "PreProcessorTokenStreamCpp.h"
#include "PreProcessorCpp.h"
#include "TokenStreamCpp.h"
#include "ConflictResolverCpp.h"
#include "ParserLALR.h"
#include "SymbolTable.h"
#include "Sentence.h"
#include "Symbol.h"

int main(int argc, char** argv)
{
	try
	{
		const auto fileName = R"(c:\save\code\CppCompiler\example.h)";
		std::ifstream in { fileName };
		CharStreamCpp chars { in, fileName };
		PreProcessorTokenStreamCpp ppTokens { chars };
		PreProcessorCpp processor { ppTokens };
		TokenStreamCpp tokens { processor };
		ConflictResolverCpp resolver;
		SymbolTable symtab;

		const auto grammarFileName = R"(c:\save\code\CppCompiler\example_grammar.h)";
		std::ifstream inGrammar { grammarFileName };
		std::stringstream bufferGrammar;
		inGrammar >> bufferGrammar.rdbuf();
		const auto grammarString = bufferGrammar.str();

		std::cout << "Using LALR parser generator on grammar." << std::endl;
		auto table = ParserLALR::Generate(grammarString, "translation-unit", resolver);

		const auto tableFileName = R"(c:\save\code\CppCompiler\output\transition.txt)";
		std::ofstream outTable { tableFileName };
		outTable << table.ToString() << std::endl;

		std::cout << "Using transition table to parse example." << std::endl;
		auto item = table.Parse(tokens, symtab, &std::cout);

		const auto xmlFileName = R"(c:\save\code\CppCompiler\output\sentence.xml)";
		std::ofstream outXml { xmlFileName };
		item->WriteXml(outXml);

		const auto symbolFileName = R"(c:\save\code\CppCompiler\output\symbols.xml)";
		std::ofstream outSymbol { symbolFileName };
		symtab.WriteXml(outSymbol);
	}
	catch (const std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
	}

	std::cout << "Press enter to continue...";
	std::cin.get();
	return 0;
}

