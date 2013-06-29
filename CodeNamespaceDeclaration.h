#pragma once
#include "CodeNode.h"
#include "CodeQualifiedName.h"
#include "CodeDeclarationList.h"

class CodeNamespaceDeclaration : public CodeNode
{
public:
	CodeNamespaceDeclaration() = default;
	CodeNamespaceDeclaration(
		const CodeQualifiedName& name,
		const CodeDeclarationList& declarationList);
	CodeNamespaceDeclaration(const CodeNamespaceDeclaration& rhs) = default;
	~CodeNamespaceDeclaration() noexcept(true) = default;

	CodeNamespaceDeclaration& operator=(const CodeNamespaceDeclaration& rhs) = default;

public:
	CodeQualifiedName name;
	CodeDeclarationList declarationList;
};

