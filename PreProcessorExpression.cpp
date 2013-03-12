#include "PreProcessorExpression.h"
#include <stdexcept>
#include "MacroExpander.h"
#include "PreProcessorExpressionOperator.h"
#include "PreProcessorExpressionNumber.h"
#include "PreProcessorExpressionUnary.h"
#include "PreProcessorExpressionBinary.h"

PreProcessorExpression::PreProcessorExpression(
	const MacroTableExpander& table,
	const std::vector<PreProcessorToken>& tokens)
	: table(table), tokens(tokens)
{
}

bool PreProcessorExpression::Evaluate()
{
	RemoveIgnoreableTokens();
	EvaluateDefinedOperator();
	tokens = MacroExpander::EvaluateSequence(table, tokens, {});
	BuildNodesFromTokens();
	ReduceNodes();

	if (nodes.size() != 1)
		throw std::logic_error("Expression did not reduce to a single node.");
	return nodes.front()->EvaluateBool();
}

void PreProcessorExpression::RemoveIgnoreableTokens()
{
	for (auto iter = tokens.begin(); iter != tokens.end(); )
	{
		if (iter->CanIgnore())
			iter = tokens.erase(iter);
		else
			++iter;
	}
}

void PreProcessorExpression::EvaluateDefinedOperator()
{
	for (auto iter = tokens.begin(); iter != tokens.end(); ++iter)
	{
		if (iter->type != PreProcessorTokenType::Identifier ||
			iter->value != "defined")
			continue;

		auto next = iter + 1;
		if (next == tokens.end())
			throw std::logic_error("defined operator must be followed by a token.");
		auto defined = false;
		if (next->type == PreProcessorTokenType::Identifier)
		{
			defined = table.GetMacroType(next->value) != MacroType::Undefined;
		}
		else if (next->IsOperator("("))
		{
			++next;
			if (next == tokens.end() || next->type != PreProcessorTokenType::Identifier)
				throw std::logic_error("defined( must be followed by an identifier.");
			defined = table.GetMacroType(next->value) != MacroType::Undefined;
			++next;
			if (next == tokens.end() || !next->IsOperator(")"))
				throw std::logic_error("defined(id must be followed by ).");
		}
		else
			throw std::logic_error("defined operator must be following by an identifier or (.");

		auto position = iter->position;
		auto value = defined ? "1" : "0";
		iter = tokens.erase(iter, next + 1);
		iter = tokens.insert(iter, { PreProcessorTokenType::Number, position, value });
	}
}

void PreProcessorExpression::BuildNodesFromTokens()
{
	for (auto& token: tokens)
	{
		if (token.CanIgnore())
			continue;

		if (token.type == PreProcessorTokenType::Operator)
			nodes.push_back(PreProcessorExpressionOperator::Create(token.value));
		else if (token.type == PreProcessorTokenType::Number)
			nodes.push_back(PreProcessorExpressionNumber::Create(token.value));
		//Replace undefined identifiers with zero
		else if (token.type == PreProcessorTokenType::Identifier)
			nodes.push_back(PreProcessorExpressionNumber::Create("0"));
		else
			throw std::logic_error("Invalid token type in expression.");
	}
}

void PreProcessorExpression::ReduceNodes()
{
	//TODO: fix operator precedence and right-to-left associativity issues.
	ReduceParenthesis();
	ReduceUnaryNodes(PreProcessorExpressionOperatorType::LogicalNot);
	ReduceBinaryNodes(PreProcessorExpressionOperatorType::GreaterThanOrEqualTo);
	ReduceBinaryNodes(PreProcessorExpressionOperatorType::GreaterThan);
	ReduceBinaryNodes(PreProcessorExpressionOperatorType::LessThanOrEqualTo);
	ReduceBinaryNodes(PreProcessorExpressionOperatorType::LessThan);
	ReduceBinaryNodes(PreProcessorExpressionOperatorType::EqualTo);
	ReduceBinaryNodes(PreProcessorExpressionOperatorType::NotEqualTo);
	ReduceBinaryNodes(PreProcessorExpressionOperatorType::Multiply);
	ReduceBinaryNodes(PreProcessorExpressionOperatorType::Divide);
	ReduceBinaryNodes(PreProcessorExpressionOperatorType::Modulus);
	ReduceBinaryNodes(PreProcessorExpressionOperatorType::Plus);
	ReduceBinaryNodes(PreProcessorExpressionOperatorType::Minus);
	ReduceBinaryNodes(PreProcessorExpressionOperatorType::LogicalAnd);
	ReduceBinaryNodes(PreProcessorExpressionOperatorType::LogicalOr);
}

void PreProcessorExpression::ReduceParenthesis()
{
	for (auto index = 0ul; index < nodes.size(); ++index)
	{
		if (nodes[index]->IsOperator(PreProcessorExpressionOperatorType::OpenParenthesis))
		{
			auto openCount = 0ul;
			auto close = index + 1;
			for (; close < nodes.size(); ++close)
			{
				if (nodes[close]->IsOperator(PreProcessorExpressionOperatorType::OpenParenthesis))
					++openCount;
				else if (nodes[close]->IsOperator(PreProcessorExpressionOperatorType::CloseParenthesis))
				{
					if (openCount == 0)
						break;
					else
						--openCount;
				}
			}
			if (openCount != 0)
				throw std::logic_error("Missing matching ).");

			auto copyOfNodes = nodes;
			nodes.erase(nodes.begin() + close, nodes.end());
			nodes.erase(nodes.begin(), nodes.begin() + index + 1);
			ReduceNodes();
			if (nodes.size() != 1)
				throw std::logic_error("Parenthesis did not reduce to single node.");
			copyOfNodes.erase(copyOfNodes.begin() + index, copyOfNodes.begin() + close + 1);
			copyOfNodes.insert(copyOfNodes.begin() + index, nodes.front());
			nodes = copyOfNodes;
		}
	}
}

void PreProcessorExpression::ReduceUnaryNodes(PreProcessorExpressionOperatorType op)
{
	for (auto iter = nodes.begin(); iter != nodes.end(); ++iter)
	{
		if ((*iter)->IsOperator(op))
		{
			auto next = iter + 1;
			if (next == nodes.end())
				throw std::logic_error("Unary operator at end of expression.");
			if (!(*next)->CanEvaluate())
				throw std::logic_error("rhs of unary operator could not be evaluated.");
			auto node = PreProcessorExpressionUnary::Create(op, *next);
			iter = nodes.erase(iter, next + 1);
			iter = nodes.insert(iter, node);
		}
	}
}

void PreProcessorExpression::ReduceBinaryNodes(PreProcessorExpressionOperatorType op)
{
	for (auto iter = nodes.begin(); iter != nodes.end(); ++iter)
	{
		if ((*iter)->IsOperator(op))
		{
			if (iter == nodes.begin())
				throw std::logic_error("Binary operator at beginning of expression.");
			auto prev = iter - 1;
			auto next = iter + 1;
			if (next == nodes.end())
				throw std::logic_error("Binary operator at end of expression.");
			if (!(*prev)->CanEvaluate())
				throw std::logic_error("lhs of binary operator could not be evaluated.");
			if (!(*next)->CanEvaluate())
				throw std::logic_error("rhs of binary operator could not be evaluated.");
			auto node = PreProcessorExpressionBinary::Create(op, *prev, *next);
			iter = nodes.erase(prev, next + 1);
			iter = nodes.insert(iter, node);
		}
	}
}

