#pragma once
#include "CodeNode.h"
#include "CodeDeclarationType.h"
#include "CodeNamespaceDeclaration.h"
#include "CodeClassDeclaration.h"

class CodeDeclaration : public CodeNode
{
public:
	CodeDeclaration() = default;
	CodeDeclaration(const CodeNamespaceDeclaration& namespaceDeclaration);
	CodeDeclaration(const CodeClassDeclaration& classDeclaration);
	CodeDeclaration(const CodeDeclaration& rhs) = default;
	~CodeDeclaration() noexcept(true) = default;

	CodeDeclaration& operator=(const CodeDeclaration& rhs) = default;

public:
	CodeDeclarationType type;
	CodeNamespaceDeclaration namespaceDeclaration;
	CodeClassDeclaration classDeclaration;
};

