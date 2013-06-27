#pragma once
#include "CodeNode.h"
#include "CodeNodeFactory.h"
#include "CodeDeclarationList.h"

class CodeTranslationUnit : public CodeNode
{
public:
	CodeTranslationUnit(const CodeDeclarationList& declarationList);

	static CodeNodeFactory::NonTerminalEntry creator;

public:
	CodeDeclarationList declarationList;
};

