#pragma once
#include "CodeTypeName.h"
#include "CodeNode.h"
#include "CodeNodeFactory.h"
#include <string>

class CodeDeclaration : public CodeNode
{
public:
	CodeDeclaration(CodeTypeName type, const std::string& name);

	static CodeNodeFactory::NonTerminalEntry creator;
	
public:
	CodeTypeName type;
	std::string name;
};

