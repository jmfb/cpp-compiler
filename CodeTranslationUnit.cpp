#include "CodeTranslationUnit.h"
#include "Sentence.h"
#include "CodeNodeRegister.h"

CodeTranslationUnit::CodeTranslationUnit(const CodeDeclarationList& declarationList)
	: declarationList(declarationList)
{
}

namespace
{
	CodeNodeRegister translationUnit
	{
		"translation-unit",
		{
			{
				"<declaration-list>",
				[](CodeNodeItems items)
				{
					return new CodeTranslationUnit
					{
						items[0]->AsCode<CodeDeclarationList>()
					};
				}
			}
		}
	};
}
