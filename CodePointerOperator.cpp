#include "CodePointerOperator.h"
#include "CodeNodeRegister.h"

CodePointerOperator::CodePointerOperator(CodePointerOperatorType type)
	: type(type)
{
}

CodePointerOperator::CodePointerOperator(CodePointerOperatorType type, bool isConst)
	: type(type), isConst(isConst)
{
}

namespace
{
	CodeNodeRegister pointerOperator
	{
		"pointer-operator",
		{
			{
				"'&'",
				[](CodeNodeItems)
				{
					return new CodePointerOperator { CodePointerOperatorType::Reference };
				}
			},
			{
				"'*'",
				[](CodeNodeItems)
				{
					return new CodePointerOperator { CodePointerOperatorType::Pointer };
				}
			},
			{
				"'?'",
				[](CodeNodeItems)
				{
					return new CodePointerOperator { CodePointerOperatorType::Nullable };
				}
			}
		}
	};
}

