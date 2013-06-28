#include "CodeTypeName.h"
#include "CodeNodeRegister.h"
#include "Sentence.h"

CodeTypeName::CodeTypeName(
	bool isConst,
	const CodeCoreTypeName& coreTypeName,
	const CodePointerSpecifier& pointerSpecifier)
	: isConst(isConst), coreTypeName(coreTypeName), pointerSpecifier(pointerSpecifier)
{
}

namespace
{
	CodeNodeRegister typeName
	{
		"type-name",
		{
			{
				"<core-type-name> <pointer-specifier-opt>",
				[](CodeNodeItems items)
				{
					return new CodeTypeName
					{
						false,
						items[0]->AsCode<CodeCoreTypeName>(),
						items[1]->AsCode<CodePointerSpecifier>()
					};
				}
			},
			{
				"\"const\" <core-type-name> <pointer-specifier-opt>",
				[](CodeNodeItems items)
				{
					return new CodeTypeName
					{
						true,
						items[1]->AsCode<CodeCoreTypeName>(),
						items[2]->AsCode<CodePointerSpecifier>()
					};
				}
			}
		}
	};
}

