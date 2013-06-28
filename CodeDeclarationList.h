#pragma once
#include "CodeNode.h"
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

public:
	std::vector<CodeDeclaration> declarations;
};

