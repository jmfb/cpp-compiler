#pragma once
#include "CodeNode.h"
#include <string>
#include <vector>

class CodeQualifiedName : public CodeNode
{
public:
	CodeQualifiedName() = default;
	CodeQualifiedName(const CodeQualifiedName& rhs) = default;
	CodeQualifiedName(
		const std::string& name,
		const std::vector<std::string>& trailingNames);
	~CodeQualifiedName() noexcept(true) = default;

	CodeQualifiedName& operator=(const CodeQualifiedName& rhs) = default;

public:
	std::vector<std::string> names;
};

