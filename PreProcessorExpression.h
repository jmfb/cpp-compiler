#pragma once
#include <vector>
#include "MacroTableExpander.h"
#include "PreProcessorToken.h"
#include "PreProcessorExpressionNode.h"

class PreProcessorExpression
{
public:
	PreProcessorExpression() = delete;
	PreProcessorExpression(
		const MacroTableExpander& table,
		const std::vector<PreProcessorToken>& tokens);
	PreProcessorExpression(const PreProcessorExpression& rhs) = default;
	~PreProcessorExpression() = default;

	PreProcessorExpression& operator=(const PreProcessorExpression& rhs) = default;

	bool Evaluate();

private:
	void RemoveIgnoreableTokens();
	void EvaluateDefinedOperator();
	void BuildNodesFromTokens();
	void ReduceNodes();
	void ReduceParenthesis();
	void ReduceUnaryNodes(PreProcessorExpressionOperatorType op);
	void ReduceBinaryNodes(PreProcessorExpressionOperatorType op);

private:
	friend class PreProcessorExpressionTest;
	const MacroTableExpander& table;
	std::vector<PreProcessorToken> tokens;
	std::vector<PreProcessorExpressionNodePtr> nodes;
};

