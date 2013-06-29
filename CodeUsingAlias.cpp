#include "CodeUsingAlias.h"
#include "CodeNodeRegister.h"

CodeUsingAlias::CodeUsingAlias(
	const std::string& alias,
	const CodeQualifiedName& name)
	: alias(alias), name(name)
{
}

namespace
{
	CodeNodeRegister usingAlias
	{
		"using-alias",
		{
			{
				"\"using\" id '=' <qualified-name> ';'",
				[](CodeNodeItems items)
				{
					return new CodeUsingAlias
					{
						items[1]->AsToken(),
						items[3]->AsCode<CodeQualifiedName>()
					};
				}
			}
		}
	};
}

