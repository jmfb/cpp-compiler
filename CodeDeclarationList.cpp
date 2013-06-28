#include "CodeDeclarationList.h"
#include "Sentence.h"
#include "CodeNodeRegister.h"

CodeDeclarationList::CodeDeclarationList(
	const CodeDeclaration& declaration,
	const CodeDeclarationList& declarationList)
{
	declarations.push_back(declaration);
	declarations.insert(
		declarations.end(),
		declarationList.declarations.begin(),
		declarationList.declarations.end());
}

namespace
{
	CodeNodeRegister declarationList
	{
		"declaration-list",
		{
			{
				"<declaration> <declaration-list-opt>",
				[](CodeNodeItems items)
				{
					return new CodeDeclarationList
					{
						items[0]->AsCode<CodeDeclaration>(),
						items[1]->AsCode<CodeDeclarationList>()
					};
				}
			}
		}
	};
	
	CodeNodeRegister declarationListOpt
	{
		"declaration-list-opt",
		{
			{
				"<declaration-list>",
				[](CodeNodeItems items)
				{
					return new CodeDeclarationList
					{
						items[0]->AsCode<CodeDeclarationList>()
					};
				}
			},
			{
				"@",
				[](CodeNodeItems)
				{
					return new CodeDeclarationList();
				}
			}
		}
	};
}

