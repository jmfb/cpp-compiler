#pragma once
#include <string>
#include "PreProcessorExpressionNode.h"

class PreProcessorExpressionNumber : public PreProcessorExpressionNode
{
public:
	PreProcessorExpressionNumber() = delete;
	PreProcessorExpressionNumber(unsigned long long value);
	PreProcessorExpressionNumber(const PreProcessorExpressionNumber& rhs) = delete;
	virtual ~PreProcessorExpressionNumber() noexcept = default;

	PreProcessorExpressionNumber& operator=(const PreProcessorExpressionNumber& rhs) = delete;

	static PreProcessorExpressionNodePtr Create(const std::string& token);

	bool CanEvaluate() const final;
	bool EvaluateBool() const final;
	unsigned long long EvaluateNumber() const final;

private:
	unsigned long long value;
};

