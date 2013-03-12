#include "PreProcessorExpressionNode.h"
#include <stdexcept>

bool PreProcessorExpressionNode::IsOperator(PreProcessorExpressionOperatorType value) const
{
	return false;
}

bool PreProcessorExpressionNode::CanEvaluate() const
{
	return false;
}

bool PreProcessorExpressionNode::EvaluateBool() const
{
	throw std::logic_error("Cannot evaluate bool.");
}

unsigned long long PreProcessorExpressionNode::EvaluateNumber() const
{
	throw std::logic_error("Cannot evaluate number.");
}

