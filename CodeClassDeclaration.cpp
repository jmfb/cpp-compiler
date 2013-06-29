#include "CodeClassDeclaration.h"
#include "CodeNodeRegister.h"

CodeClassDeclaration::CodeClassDeclaration(
	const CodeQualifiedName& name)
	: name(name)
{
}

namespace
{
	CodeNodeRegister classDeclaration
	{
		"class-declaration",
		{
			{
				"\"class\" <qualified-name> '{' '}'",
				[](CodeNodeItems items)
				{
					return new CodeClassDeclaration
					{
						items[1]->AsCode<CodeQualifiedName>()
					};
				}
			}
		}
	};
}

