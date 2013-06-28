#include <fstream>
#include <iostream>
#include <sstream>
#include <exception>

#include "CharStreamDiamond.h"
#include "PreProcessorTokenStreamDiamond.h"
#include "PreProcessorDiamond.h"
#include "TokenStreamDiamond.h"
#include "ConflictResolverDiamond.h"
#include "CodeNodeFactory.h"
#include "CodeTranslationUnit.h"

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
		CharStreamDiamond chars { in, fileName };
		PreProcessorTokenStreamDiamond ppTokens { chars };
		PreProcessorDiamond processor { ppTokens };
		TokenStreamDiamond tokens { processor };
		ConflictResolverDiamond resolver;
		SymbolTable symtab;

		std::cout << "Using CodeNodeFactory to generate grammar string." << std::endl;
		auto grammarString = CodeNodeFactory::GenerateGrammar();
		const auto grammarFileName = R"(c:\save\code\CppCompiler\output\_grammar.txt)";
		std::ofstream outGrammar { grammarFileName };
		outGrammar << grammarString << std::endl;

		std::cout << "Using LALR parser generator on grammar." << std::endl;
		auto table = ParserLALR::Generate(grammarString, "translation-unit", resolver);

		const auto tableFileName = R"(c:\save\code\CppCompiler\output\_transition.txt)";
		std::ofstream outTable { tableFileName };
		outTable << table.ToString() << std::endl;

		std::cout << "Using transition table to parse example." << std::endl;
		auto item = table.Parse(tokens, symtab, &std::cout);

		const auto xmlFileName = R"(c:\save\code\CppCompiler\output\_sentence.xml)";
		std::ofstream outXml { xmlFileName };
		item->WriteXml(outXml);

		const auto symbolFileName = R"(c:\save\code\CppCompiler\output\_symbols.xml)";
		std::ofstream outSymbol { symbolFileName };
		symtab.WriteXml(outSymbol);

		auto& translationUnit = item->AsCode<CodeTranslationUnit>();
		for (auto& declaration: translationUnit.declarationList.declarations)
		{
			std::cout << "Declared: " << declaration.name << std::endl;
			if (declaration.typeName.coreTypeName.type == CodeCoreType::QualifiedName)
			{
				std::cout << "  Type: ";
				for (auto& name: declaration.typeName.coreTypeName.qualifiedName.names)
					std::cout << name << " ";
				std::cout << std::endl;
			}
		}
	}
	catch (const std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
	}

	std::cout << "Press enter to continue...";
	std::cin.get();
	return 0;
}

