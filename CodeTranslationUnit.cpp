#include "CodeTranslationUnit.h"
#include "Sentence.h"

CodeTranslationUnit::CodeTranslationUnit(const CodeDeclarationList& declarationList)
	: declarationList(declarationList)
{
}

CodeNodeFactory::NonTerminalEntry CodeTranslationUnit::creator
{
	"translation-unit",
	{
		{
			"<declaration-list>",
			[](CodeNodeFactory::Items items)
			{
				return CodeNodePtr
				{
					new CodeTranslationUnit
					{
						items[0]->AsCode<CodeDeclarationList>()
					}
				};
			}
		}
	}
};

