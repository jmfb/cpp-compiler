#pragma once
#include <string>
#include "PreProcessorExpressionNode.h"
#include "PreProcessorExpressionOperatorType.h"

class PreProcessorExpressionOperator : public PreProcessorExpressionNode
{
public:
	PreProcessorExpressionOperator() = delete;
	PreProcessorExpressionOperator(PreProcessorExpressionOperatorType value);
	PreProcessorExpressionOperator(const PreProcessorExpressionOperator& rhs) = delete;
	~PreProcessorExpressionOperator() noexcept = default;

	PreProcessorExpressionOperator& operator=(const PreProcessorExpressionOperator& rhs) = delete;

	static PreProcessorExpressionNodePtr Create(const std::string& token);

	bool IsOperator(PreProcessorExpressionOperatorType value) const final;

private:
	PreProcessorExpressionOperatorType value;
};

