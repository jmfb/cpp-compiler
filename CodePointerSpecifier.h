#pragma once
#include "CodeNode.h"
#include <vector>
#include "CodePointerOperatorType.h"
#include "CodePointerOperator.h"

class CodePointerSpecifier : public CodeNode
{
public:
	CodePointerSpecifier() = default;
	CodePointerSpecifier(const CodePointerSpecifier& rhs) = default;
	CodePointerSpecifier(
		CodePointerOperatorType type,
		bool isConst,
		const std::vector<CodePointerOperator>& trailingSpecifier);

public:
	std::vector<CodePointerOperator> pointerOperators;
};

