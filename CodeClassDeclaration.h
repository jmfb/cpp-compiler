#pragma once
#include "CodeNode.h"
#include "CodeQualifiedName.h"

class CodeClassDeclaration : public CodeNode
{
public:
	CodeClassDeclaration() = default;
	CodeClassDeclaration(
		const CodeQualifiedName& name);
	CodeClassDeclaration(const CodeClassDeclaration& rhs) = default;
	~CodeClassDeclaration() noexcept(true) = default;

	CodeClassDeclaration& operator=(const CodeClassDeclaration& rhs) = default;

public:
	CodeQualifiedName name;
};

