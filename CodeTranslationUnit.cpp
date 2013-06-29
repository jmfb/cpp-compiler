#include "CodeTranslationUnit.h"
#include "CodeNodeRegister.h"
#include "CodeDeclaration.h"

CodeTranslationUnit::CodeTranslationUnit(
	const CodeUsingList& usingList,
	const CodeDeclarationList& declarationList)
	: usingList(usingList), declarationList(declarationList)
{
}

namespace
{
	CodeNodeRegister translationUnit
	{
		"translation-unit",
		{
			{
				"<using-list-opt> <declaration-list>",
				[](CodeNodeItems items)
				{
					return new CodeTranslationUnit
					{
						items[0]->AsCode<CodeUsingList>(),
						items[1]->AsCode<CodeDeclarationList>()
					};
				}
			}
		}
	};
}

