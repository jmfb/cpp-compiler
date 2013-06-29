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

#define MAP_OP(op,opEnum) \
	{ \
		"'" op "'", \
		[](CodeNodeItems) \
		{ \
			return new CodePointerOperator { CodePointerOperatorType::opEnum }; \
		} \
	}

namespace
{
	CodeNodeRegister pointerOperator
	{
		"pointer-operator",
		{
			MAP_OP("&", Reference),
			MAP_OP("*", Pointer),
			MAP_OP("?", Nullable)
		}
	};
}

