#include "CodeDeclaration.h"
#include "CodeNodeRegister.h"

CodeDeclaration::CodeDeclaration(const CodeNamespaceDeclaration& namespaceDeclaration)
	: type(CodeDeclarationType::Namespace), namespaceDeclaration(namespaceDeclaration)
{
}

CodeDeclaration::CodeDeclaration(const CodeClassDeclaration& classDeclaration)
	: type(CodeDeclarationType::Class), classDeclaration(classDeclaration)
{
}

namespace
{
	CodeNodeRegister declaration
	{
		"declaration",
		{
			{
				"<namespace-declaration>",
				[](CodeNodeItems items)
				{
					return new CodeDeclaration
					{
						items[0]->AsCode<CodeNamespaceDeclaration>()
					};
				}
			},
			{
				"<class-declaration>",
				[](CodeNodeItems items)
				{
					return new CodeDeclaration
					{
						items[0]->AsCode<CodeClassDeclaration>()
					};
				}
			}
		}
	};
}

