#pragma once
#include "PreProcessorExpressionNode.h"
#include "PreProcessorExpressionOperatorType.h"

class PreProcessorExpressionUnary : public PreProcessorExpressionNode
{
public:
	PreProcessorExpressionUnary() = delete;
	PreProcessorExpressionUnary(
		PreProcessorExpressionOperatorType op,
		PreProcessorExpressionNodePtr value);
	PreProcessorExpressionUnary(const PreProcessorExpressionUnary& rhs) = delete;
	~PreProcessorExpressionUnary() noexcept = default;

	PreProcessorExpressionUnary& operator=(const PreProcessorExpressionUnary& rhs) = delete;

	static PreProcessorExpressionNodePtr Create(
		PreProcessorExpressionOperatorType op,
		PreProcessorExpressionNodePtr value);

	bool CanEvaluate() const final;
	bool EvaluateBool() const final;
	unsigned long long EvaluateNumber() const final;

private:
	PreProcessorExpressionOperatorType op;
	PreProcessorExpressionNodePtr value;
};

