#include "CodeUsingList.h"
#include "CodeNodeRegister.h"

CodeUsingList::CodeUsingList(CodeNodePtr usingStatement)
	: usingList { usingStatement }
{
}

CodeUsingList::CodeUsingList(
	CodeNodePtr usingStatement,
	const std::vector<CodeNodePtr> trailingList)
	: usingList { usingStatement }
{
	usingList.insert(
		usingList.end(),
		trailingList.begin(),
		trailingList.end());
}

namespace
{
	CodeNodeRegister usingList
	{
		"using-list",
		{
			{
				"<using-statement> <using-list-opt>",
				[](CodeNodeItems items)
				{
					return new CodeUsingList
					{
						items[0]->AsCode<CodeUsingList>().usingList[0],
						items[1]->AsCode<CodeUsingList>().usingList
					};
				}
			}
		}
	};

	CodeNodeRegister usingListOpt
	{
		"using-list-opt",
		{
			{
				"<using-list>",
				[](CodeNodeItems items)
				{
					return new CodeUsingList { items[0]->AsCode<CodeUsingList>() };
				}
			},
			{
				"@",
				[](CodeNodeItems items)
				{
					return new CodeUsingList { };
				}
			}
		}
	};

	CodeNodeRegister usingStatement
	{
		"using-statement",
		{
			{
				"<using-import>",
				[](CodeNodeItems items)
				{
					return new CodeUsingList { items[0]->AsNode() };
				}
			},
			{
				"<using-alias>",
				[](CodeNodeItems items)
				{
					return new CodeUsingList { items[0]->AsNode() };
				}
			}
		}
	};
}

