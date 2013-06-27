#pragma once
#include "CodeNode.h"
#include "CodeNodeFactory.h"
#include "CodeDeclaration.h"
#include <vector>

class CodeDeclarationList : public CodeNode
{
public:
	CodeDeclarationList() = default;
	CodeDeclarationList(const CodeDeclarationList& rhs) = default;
	CodeDeclarationList(
		const CodeDeclaration& declaration,
		const CodeDeclarationList& declarationList);

	static CodeNodeFactory::NonTerminalEntry creator;
	static CodeNodeFactory::NonTerminalEntry optionalCreator;

public:
	std::vector<CodeDeclaration> declarations;
};

