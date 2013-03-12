#include "PreProcessorExpressionOperator.h"
#include <stdexcept>

PreProcessorExpressionOperator::PreProcessorExpressionOperator(
	PreProcessorExpressionOperatorType value)
	: value(value)
{
}

PreProcessorExpressionNodePtr PreProcessorExpressionOperator::Create(
	const std::string& token)
{
	auto value = PreProcessorExpressionOperatorType::LogicalNot;
	if (token == "&&")
		value = PreProcessorExpressionOperatorType::LogicalAnd;
	else if (token == "||")
		value = PreProcessorExpressionOperatorType::LogicalOr;
	else if (token == "!")
		value = PreProcessorExpressionOperatorType::LogicalNot;
	else if (token == ">=")
		value = PreProcessorExpressionOperatorType::GreaterThanOrEqualTo;
	else if (token == ">")
		value = PreProcessorExpressionOperatorType::GreaterThan;
	else if (token == "<=")
		value = PreProcessorExpressionOperatorType::LessThanOrEqualTo;
	else if (token == "<")
		value = PreProcessorExpressionOperatorType::LessThan;
	else if (token == "==")
		value = PreProcessorExpressionOperatorType::EqualTo;
	else if (token == "!=")
		value = PreProcessorExpressionOperatorType::NotEqualTo;
	else if (token == "+")
		value = PreProcessorExpressionOperatorType::Plus;
	else if (token == "-")
		value = PreProcessorExpressionOperatorType::Minus;
	else if (token == "*")
		value = PreProcessorExpressionOperatorType::Multiply;
	else if (token == "/")
		value = PreProcessorExpressionOperatorType::Divide;
	else if (token == "%")
		value = PreProcessorExpressionOperatorType::Modulus;
	else if (token == "(")
		value = PreProcessorExpressionOperatorType::OpenParenthesis;
	else if (token == ")")
		value = PreProcessorExpressionOperatorType::CloseParenthesis;
	else
		throw std::logic_error("Invalid operator in expression.");
	return PreProcessorExpressionNodePtr { new PreProcessorExpressionOperator { value } };
}

bool PreProcessorExpressionOperator::IsOperator(
	PreProcessorExpressionOperatorType value) const
{
	return value == this->value;
}

