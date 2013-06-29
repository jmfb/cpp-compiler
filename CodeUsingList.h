#pragma once
#include "CodeNode.h"
#include <vector>

class CodeUsingList : public CodeNode
{
public:
	CodeUsingList() = default;
	CodeUsingList(CodeNodePtr usingStatement);
	CodeUsingList(
		CodeNodePtr usingStatement,
		const std::vector<CodeNodePtr> trailingList);
	CodeUsingList(const CodeUsingList& rhs) = default;
	~CodeUsingList() noexcept(true) = default;

	CodeUsingList& operator=(const CodeUsingList& rhs) = default;

public:
	std::vector<CodeNodePtr> usingList;
};

