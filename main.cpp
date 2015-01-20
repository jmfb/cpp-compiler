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

#include "CodeDeclaration.h"

void Test(CodeTranslationUnit& translationUnit);

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

		//std::cout << "Using CodeNodeFactory to generate grammar string." << std::endl;
		//auto grammarString = CodeNodeFactory::GenerateGrammar();
		//const auto grammarFileName = R"(c:\save\code\CppCompiler\output\_grammar.txt)";
		//std::ofstream outGrammar { grammarFileName };
		//outGrammar << grammarString << std::endl;
		const auto grammarFileName = R"(c:\save\code\CppCompiler\diamond_grammar.h)";
		std::ifstream inGrammar { grammarFileName };
		std::ostringstream outGrammar;
		inGrammar >> outGrammar.rdbuf();
		auto grammarString = outGrammar.str();

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

		Test(item->AsCode<CodeTranslationUnit>());
	}
	catch (const std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
	}

	std::cout << "Press enter to continue...";
	std::cin.get();
	return 0;
}

class Writer
{
public:
	static void Write(const CodeNamespaceDeclaration& namespaceDeclaration)
	{
		for (auto name: namespaceDeclaration.name.names)
			std::cout << "namespace " << name << " { ";
		for (auto& declaration: namespaceDeclaration.declarationList.declarations)
			Write(declaration);
		for (auto name: namespaceDeclaration.name.names)
			std::cout << "} ";
	}

	static void Write(const CodeClassDeclaration& classDeclaration)
	{
		for (auto index = 1ul; index < classDeclaration.name.names.size(); ++index)
			std::cout << "namespace " << classDeclaration.name.names[index - 1] << " { ";
		std::cout << "class " << classDeclaration.name.names.back() << " { } ; ";
		for (auto index = 1ul; index < classDeclaration.name.names.size(); ++index)
			std::cout << "} ";
	}

	static void Write(const CodeDeclaration& declaration)
	{
		switch(declaration.type)
		{
		case CodeDeclarationType::Namespace:
			Write(declaration.namespaceDeclaration);
			break;

		case CodeDeclarationType::Class:
			Write(declaration.classDeclaration);
			break;
		}
	}
};

void Test(CodeTranslationUnit& translationUnit)
{
	for (auto& declaration: translationUnit.declarationList.declarations)
		Writer::Write(declaration);
}

