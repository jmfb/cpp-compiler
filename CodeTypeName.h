#pragma once
#include "CodeNode.h"
#include "CodeCoreTypeName.h"
#include "CodePointerSpecifier.h"

class CodeTypeName : public CodeNode
{
public:
	CodeTypeName(
		bool isConst,
		const CodeCoreTypeName& coreTypeName,
		const CodePointerSpecifier& pointerSpecifier);
	~CodeTypeName() noexcept(true) = default;

public:
	bool isConst;
	CodeCoreTypeName coreTypeName;
	CodePointerSpecifier pointerSpecifier;
};

