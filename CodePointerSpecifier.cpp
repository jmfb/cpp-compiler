#include "CodePointerSpecifier.h"
#include "CodeNodeRegister.h"

CodePointerSpecifier::CodePointerSpecifier(
	CodePointerOperatorType type,
	bool isConst,
	const std::vector<CodePointerOperator>& trailingSpecifier)
	: pointerOperators { { type, isConst } }
{
	pointerOperators.insert(
		pointerOperators.end(),
		trailingSpecifier.begin(),
		trailingSpecifier.end());
}

namespace
{
	CodeNodeRegister pointerSpecifier
	{
		"pointer-specifier",
		{
			{
				"<pointer-operator> <pointer-specifier-opt>",
				[](CodeNodeItems items)
				{
					return new CodePointerSpecifier
					{
						items[0]->AsCode<CodePointerOperator>().type,
						false,
						items[1]->AsCode<CodePointerSpecifier>().pointerOperators
					};
				}
			},
			{
				"<pointer-operator> \"const\" <pointer-specifier-opt>",
				[](CodeNodeItems items)
				{
					return new CodePointerSpecifier
					{
						items[0]->AsCode<CodePointerOperator>().type,
						true,
						items[2]->AsCode<CodePointerSpecifier>().pointerOperators
					};
				}
			}
		}
	};

	CodeNodeRegister pointerSpecifierOpt
	{
		"pointer-specifier-opt",
		{
			{
				"<pointer-specifier>",
				[](CodeNodeItems items)
				{
					return new CodePointerSpecifier
					{
						items[0]->AsCode<CodePointerSpecifier>()
					};
				}
			},
			{
				"@",
				[](CodeNodeItems items)
				{
					return new CodePointerSpecifier();
				}
			}
		}
	};
}

