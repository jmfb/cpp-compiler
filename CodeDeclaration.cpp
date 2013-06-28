#include "CodeDeclaration.h"
#include "Sentence.h"
#include "CodeNodeRegister.h"

CodeDeclaration::CodeDeclaration(CodeTypeName typeName, const std::string& name)
	: typeName(typeName), name(name)
{
}

namespace
{
	CodeNodeRegister declaration
	{
		"declaration",
		{
			{
				"<type-name> id ';'",
				[](CodeNodeItems items)
				{
					return new CodeDeclaration
					{
						items[0]->AsCode<CodeTypeName>(),
						items[1]->AsToken()
					};
				}
			}
		}
	};
}

