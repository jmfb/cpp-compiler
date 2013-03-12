#include "PreProcessorExpressionNumber.h"
#include <sstream>

PreProcessorExpressionNumber::PreProcessorExpressionNumber(unsigned long long value)
	: value(value)
{
}

PreProcessorExpressionNodePtr PreProcessorExpressionNumber::Create(const std::string& token)
{
	std::istringstream in { token };
	auto value = 0ull;
	in >> value;
	return PreProcessorExpressionNodePtr { new PreProcessorExpressionNumber { value } };
}

bool PreProcessorExpressionNumber::CanEvaluate() const
{
	return true;
}

bool PreProcessorExpressionNumber::EvaluateBool() const
{
	return value != 0;
}

unsigned long long PreProcessorExpressionNumber::EvaluateNumber() const
{
	return value;
}

