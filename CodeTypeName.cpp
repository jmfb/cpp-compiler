#include "CodeTypeName.h"

CodeTypeName::CodeTypeName(CodeTypeNameType type)
	: type(type)
{
}

CodeNodeFactory::NonTerminalEntry CodeTypeName::creator
{
	"type-name",
	{
		{
			"\"var\"",
			[](CodeNodeFactory::Items)
			{
				return CodeNodePtr { new CodeTypeName { CodeTypeNameType::Var } };
			}
		},
		{
			"\"bool\"",
			[](CodeNodeFactory::Items)
			{
				return CodeNodePtr { new CodeTypeName { CodeTypeNameType::Bool } };
			}
		}
	}
};

