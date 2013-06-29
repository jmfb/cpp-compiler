#include "CodeCoreTypeName.h"
#include "CodeNodeRegister.h"

CodeCoreTypeName::CodeCoreTypeName(const CodeBuiltInTypeName& builtInTypeName)
	: type(CodeCoreType::BuiltInTypeName), builtInTypeName(builtInTypeName)
{
}

CodeCoreTypeName::CodeCoreTypeName(const CodeQualifiedName& qualifiedName)
	: type(CodeCoreType::QualifiedName), qualifiedName(qualifiedName)
{
}

namespace
{
	CodeNodeRegister coreTypeName
	{
		"core-type-name",
		{
			{
				"<built-in-type-name>",
				[](CodeNodeItems items)
				{
					return new CodeCoreTypeName
					{
						items[0]->AsCode<CodeBuiltInTypeName>()
					};
				}
			},
			{
				"<qualified-name>",
				[](CodeNodeItems items)
				{
					return new CodeCoreTypeName
					{
						items[0]->AsCode<CodeQualifiedName>()
					};
				}
			}
		}
	};
}

