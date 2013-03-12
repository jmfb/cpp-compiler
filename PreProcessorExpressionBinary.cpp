#include "PreProcessorExpressionBinary.h"
#include <stdexcept>

PreProcessorExpressionBinary::PreProcessorExpressionBinary(
	PreProcessorExpressionOperatorType op,
	PreProcessorExpressionNodePtr lhs,
	PreProcessorExpressionNodePtr rhs)
	: op(op), lhs(lhs), rhs(rhs)
{
}

PreProcessorExpressionNodePtr PreProcessorExpressionBinary::Create(
	PreProcessorExpressionOperatorType op,
	PreProcessorExpressionNodePtr lhs,
	PreProcessorExpressionNodePtr rhs)
{
	return PreProcessorExpressionNodePtr { new PreProcessorExpressionBinary { op, lhs, rhs } };
}

bool PreProcessorExpressionBinary::CanEvaluate() const
{
	return true;
}

bool PreProcessorExpressionBinary::EvaluateBool() const
{
	switch(op)
	{
	case PreProcessorExpressionOperatorType::LogicalAnd:
		return lhs->EvaluateBool() && rhs->EvaluateBool();
	case PreProcessorExpressionOperatorType::LogicalOr:
		return lhs->EvaluateBool() || rhs->EvaluateBool();
	case PreProcessorExpressionOperatorType::GreaterThanOrEqualTo:
		return lhs->EvaluateNumber() >= rhs->EvaluateNumber();
	case PreProcessorExpressionOperatorType::GreaterThan:
		return lhs->EvaluateNumber() > rhs->EvaluateNumber();
	case PreProcessorExpressionOperatorType::LessThanOrEqualTo:
		return lhs->EvaluateNumber() <= rhs->EvaluateNumber();
	case PreProcessorExpressionOperatorType::LessThan:
		return lhs->EvaluateNumber() < rhs->EvaluateNumber();
	case PreProcessorExpressionOperatorType::EqualTo:
		return lhs->EvaluateNumber() == rhs->EvaluateNumber();
	case PreProcessorExpressionOperatorType::NotEqualTo:
		return lhs->EvaluateNumber() != rhs->EvaluateNumber();
	case PreProcessorExpressionOperatorType::Plus:
		return (lhs->EvaluateNumber() + rhs->EvaluateNumber()) != 0;
	case PreProcessorExpressionOperatorType::Minus:
		return (lhs->EvaluateNumber() - rhs->EvaluateNumber()) != 0;
	case PreProcessorExpressionOperatorType::Multiply:
		return (lhs->EvaluateNumber() * rhs->EvaluateNumber()) != 0;
	case PreProcessorExpressionOperatorType::Divide:
		return (lhs->EvaluateNumber() / rhs->EvaluateNumber()) != 0;
	case PreProcessorExpressionOperatorType::Modulus:
		return (lhs->EvaluateNumber() % rhs->EvaluateNumber()) != 0;
	default:
		throw std::logic_error("Binary operator not supported.");
	}
}

unsigned long long PreProcessorExpressionBinary::EvaluateNumber() const
{
	switch(op)
	{
	case PreProcessorExpressionOperatorType::LogicalAnd:
		return lhs->EvaluateBool() && rhs->EvaluateBool() ? 1 : 0;
	case PreProcessorExpressionOperatorType::LogicalOr:
		return lhs->EvaluateBool() || rhs->EvaluateBool() ? 1 : 0;
	case PreProcessorExpressionOperatorType::GreaterThanOrEqualTo:
		return lhs->EvaluateNumber() >= rhs->EvaluateNumber() ? 1 : 0;
	case PreProcessorExpressionOperatorType::GreaterThan:
		return lhs->EvaluateNumber() > rhs->EvaluateNumber() ? 1 : 0;
	case PreProcessorExpressionOperatorType::LessThanOrEqualTo:
		return lhs->EvaluateNumber() <= rhs->EvaluateNumber() ? 1 : 0;
	case PreProcessorExpressionOperatorType::LessThan:
		return lhs->EvaluateNumber() < rhs->EvaluateNumber() ? 1 : 0;
	case PreProcessorExpressionOperatorType::EqualTo:
		return lhs->EvaluateNumber() == rhs->EvaluateNumber() ? 1 : 0;
	case PreProcessorExpressionOperatorType::NotEqualTo:
		return lhs->EvaluateNumber() != rhs->EvaluateNumber() ? 1 : 0;
	case PreProcessorExpressionOperatorType::Plus:
		return lhs->EvaluateNumber() + rhs->EvaluateNumber();
	case PreProcessorExpressionOperatorType::Minus:
		return lhs->EvaluateNumber() - rhs->EvaluateNumber();
	case PreProcessorExpressionOperatorType::Multiply:
		return lhs->EvaluateNumber() * rhs->EvaluateNumber();
	case PreProcessorExpressionOperatorType::Divide:
		return lhs->EvaluateNumber() / rhs->EvaluateNumber();
	case PreProcessorExpressionOperatorType::Modulus:
		return lhs->EvaluateNumber() % rhs->EvaluateNumber();
	default:
		throw std::logic_error("Binary operator not supported.");
	}
}

