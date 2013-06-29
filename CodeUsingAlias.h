#pragma once
#include "CodeNode.h"
#include "CodeQualifiedName.h"
#include <string>

class CodeUsingAlias : public CodeNode
{
public:
	CodeUsingAlias() = default;
	CodeUsingAlias(
		const std::string& alias,
		const CodeQualifiedName& name);
	CodeUsingAlias(const CodeUsingAlias& rhs) = default;
	~CodeUsingAlias() noexcept(true) = default;

	CodeUsingAlias& operator=(const CodeUsingAlias& rhs) = default;

public:
	std::string alias;
	CodeQualifiedName name;
};

