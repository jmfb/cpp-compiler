#pragma once
#include "CodeNode.h"
#include "CodeUsingList.h"
#include "CodeDeclarationList.h"

class CodeTranslationUnit : public CodeNode
{
public:
	CodeTranslationUnit() = default;
	CodeTranslationUnit(
		const CodeUsingList& usingList,
		const CodeDeclarationList& declarationList);
	CodeTranslationUnit(const CodeTranslationUnit& rhs) = default;
	~CodeTranslationUnit() noexcept(true) = default;

	CodeTranslationUnit& operator=(const CodeTranslationUnit& rhs) = default;

public:
	CodeUsingList usingList;
	CodeDeclarationList declarationList;
};

