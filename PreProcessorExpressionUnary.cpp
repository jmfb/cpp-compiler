#include "PreProcessorExpressionUnary.h"
#include <stdexcept>

PreProcessorExpressionUnary::PreProcessorExpressionUnary(
	PreProcessorExpressionOperatorType op,
	PreProcessorExpressionNodePtr value)
	: op(op), value(value)
{
}

PreProcessorExpressionNodePtr PreProcessorExpressionUnary::Create(
	PreProcessorExpressionOperatorType op,
	PreProcessorExpressionNodePtr value)
{
	return PreProcessorExpressionNodePtr { new PreProcessorExpressionUnary { op, value } };
}

bool PreProcessorExpressionUnary::CanEvaluate() const
{
	return true;
}

bool PreProcessorExpressionUnary::EvaluateBool() const
{
	switch(op)
	{
	case PreProcessorExpressionOperatorType::LogicalNot:
		return !value->EvaluateBool();
	default:
		throw std::logic_error("Unary operator not supported.");
	}
}

unsigned long long PreProcessorExpressionUnary::EvaluateNumber() const
{
	switch(op)
	{
	case PreProcessorExpressionOperatorType::LogicalNot:
		return !value->EvaluateBool() ? 1 : 0;
	default:
		throw std::logic_error("Unary operator not supported.");
	}
}

