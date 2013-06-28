#pragma once
#include "CodeTypeName.h"
#include "CodeNode.h"
#include <string>

class CodeDeclaration : public CodeNode
{
public:
	CodeDeclaration(CodeTypeName typeName, const std::string& name);

public:
	CodeTypeName typeName;
	std::string name;
};

