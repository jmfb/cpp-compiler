#include "CodeDeclarationList.h"
#include "Sentence.h"

CodeDeclarationList::CodeDeclarationList(
	const CodeDeclaration& declaration,
	const CodeDeclarationList& declarationList)
{
	declarations.push_back(declaration);
	declarations.insert(declarations.end(), declarationList.declarations.begin(), declarationList.declarations.end());
}

CodeNodeFactory::NonTerminalEntry CodeDeclarationList::creator
{
	"declaration-list",
	{
		{
			"<declaration> <declaration-list-opt>",
			[](CodeNodeFactory::Items items)
			{
				return CodeNodePtr
				{
					new CodeDeclarationList
					{
						items[0]->AsCode<CodeDeclaration>(),
						items[1]->AsCode<CodeDeclarationList>()
					}
				};
			}
		}
	}
};

CodeNodeFactory::NonTerminalEntry CodeDeclarationList::optionalCreator
{
	"declaration-list-opt",
	{
		{
			"<declaration-list>",
			[](CodeNodeFactory::Items items)
			{
				return CodeNodePtr
				{
					new CodeDeclarationList
					{
						items[0]->AsCode<CodeDeclarationList>()
					}
				};
			}
		},
		{
			"@",
			[](CodeNodeFactory::Items)
			{
				return CodeNodePtr { new CodeDeclarationList() };
			}
		}
	}
};

