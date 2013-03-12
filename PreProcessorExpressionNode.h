#pragma once
#include <memory>
#include "PreProcessorExpressionOperatorType.h"

class PreProcessorExpressionNode
{
public:
	PreProcessorExpressionNode() = default;
	PreProcessorExpressionNode(const PreProcessorExpressionNode& rhs) = delete;
	virtual ~PreProcessorExpressionNode() = default;

	PreProcessorExpressionNode& operator=(const PreProcessorExpressionNode& rhs) = delete;

	virtual bool IsOperator(PreProcessorExpressionOperatorType value) const;
	virtual bool CanEvaluate() const;
	virtual bool EvaluateBool() const;
	virtual unsigned long long EvaluateNumber() const;
};

typedef std::shared_ptr<PreProcessorExpressionNode> PreProcessorExpressionNodePtr;

