#pragma once
#include "CodeNode.h"
#include "CodeDeclarationList.h"

class CodeTranslationUnit : public CodeNode
{
public:
	CodeTranslationUnit(const CodeDeclarationList& declarationList);

public:
	CodeDeclarationList declarationList;
};

