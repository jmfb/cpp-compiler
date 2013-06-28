#pragma once
#include "CodeNode.h"
#include "CodeBuiltInType.h"

class CodeBuiltInTypeName : public CodeNode
{
public:
	CodeBuiltInTypeName() = default;
	CodeBuiltInTypeName(CodeBuiltInType type);
	CodeBuiltInTypeName(const CodeBuiltInTypeName& rhs) = default;
	~CodeBuiltInTypeName() noexcept(true) = default;

	CodeBuiltInTypeName& operator=(const CodeBuiltInTypeName& rhs) = default;

public:
	CodeBuiltInType type;
};

