#include "CodeNamespaceDeclaration.h"
#include "CodeNodeRegister.h"
#include "CodeDeclaration.h"

CodeNamespaceDeclaration::CodeNamespaceDeclaration(
	const CodeQualifiedName& name,
	const CodeDeclarationList& declarationList)
	: name(name), declarationList(declarationList)
{
}

namespace
{
	CodeNodeRegister namespaceDeclaration
	{
		"namespace-declaration",
		{
			{
				"\"namespace\" <qualified-name> '{' <declaration-list-opt> '}'",
				[](CodeNodeItems items)
				{
					return new CodeNamespaceDeclaration
					{
						items[1]->AsCode<CodeQualifiedName>(),
						items[3]->AsCode<CodeDeclarationList>()
					};
				}
			}
		}
	};
}

