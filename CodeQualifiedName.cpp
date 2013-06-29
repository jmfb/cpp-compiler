#include "CodeQualifiedName.h"
#include "CodeNodeRegister.h"

CodeQualifiedName::CodeQualifiedName(
	const std::string& name,
	const std::vector<std::string>& trailingNames)
	: names { name }
{
	names.insert(names.end(), trailingNames.begin(), trailingNames.end());
}

namespace
{
	CodeNodeRegister qualifiedName
	{
		"qualified-name",
		{
			{
				"id <qualified-suffix-opt>",
				[](CodeNodeItems items)
				{
					return new CodeQualifiedName
					{
						items[0]->AsToken(),
						items[1]->AsCode<CodeQualifiedName>().names
					};
				}
			}
		}
	};

	CodeNodeRegister qualifiedSuffix
	{
		"qualified-suffix",
		{
			{
				"'::' id <qualified-suffix-opt>",
				[](CodeNodeItems items)
				{
					return new CodeQualifiedName
					{
						items[1]->AsToken(),
						items[2]->AsCode<CodeQualifiedName>().names
					};
				}
			}
		}
	};

	CodeNodeRegister qualifiedSuffixOpt
	{
		"qualified-suffix-opt",
		{
			{
				"<qualified-suffix>",
				[](CodeNodeItems items)
				{
					return new CodeQualifiedName
					{
						items[0]->AsCode<CodeQualifiedName>()
					};
				}
			},
			{
				"@",
				[](CodeNodeItems)
				{
					return new CodeQualifiedName();
				}
			}
		}
	};
}

