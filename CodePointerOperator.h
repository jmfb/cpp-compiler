#pragma once
#include "CodeNode.h"
#include "CodePointerOperatorType.h"

class CodePointerOperator : public CodeNode
{
public:
	CodePointerOperator(CodePointerOperatorType type);
	CodePointerOperator(CodePointerOperatorType type, bool isConst);

public:
	CodePointerOperatorType type;
	bool isConst = false;
};

