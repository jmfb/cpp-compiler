#pragma once
#include "PreProcessorExpressionNode.h"
#include "PreProcessorExpressionOperatorType.h"

class PreProcessorExpressionBinary : public PreProcessorExpressionNode
{
public:
	PreProcessorExpressionBinary(
		PreProcessorExpressionOperatorType op,
		PreProcessorExpressionNodePtr lhs,
		PreProcessorExpressionNodePtr rhs);

	static PreProcessorExpressionNodePtr Create(
		PreProcessorExpressionOperatorType op,
		PreProcessorExpressionNodePtr lhs,
		PreProcessorExpressionNodePtr rhs);

	bool CanEvaluate() const final;
	bool EvaluateBool() const final;
	unsigned long long EvaluateNumber() const final;

private:
	PreProcessorExpressionOperatorType op;
	PreProcessorExpressionNodePtr lhs;
	PreProcessorExpressionNodePtr rhs;
};

