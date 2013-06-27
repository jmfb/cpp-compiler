#pragma once
#include "CodeNode.h"
#include "CodeTypeNameType.h"
#include "CodeNodeFactory.h"

class CodeTypeName : public CodeNode
{
public:
	CodeTypeName(CodeTypeNameType type);

	static CodeNodeFactory::NonTerminalEntry creator;
	
public:
	CodeTypeNameType type;
};

