#pragma once
#include "CodeNode.h"
#include "CodeQualifiedName.h"

class CodeUsingImport : public CodeNode
{
public:
	CodeUsingImport() = default;
	CodeUsingImport(const CodeQualifiedName& name);
	CodeUsingImport(const CodeUsingImport& rhs) = default;
	~CodeUsingImport() noexcept(true) = default;

	CodeUsingImport& operator=(const CodeUsingImport& rhs) = default;

public:
	CodeQualifiedName name;
};

