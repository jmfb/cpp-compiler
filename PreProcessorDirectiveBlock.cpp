#include "PreProcessorDirectiveBlock.h"

PreProcessorDirectiveBlock::PreProcessorDirectiveBlock(
	PreProcessorDirectiveType type,
	const Position& position,
	const std::vector<PreProcessorToken>& trailingTokens)
	: type(type), position(position), trailingTokens(trailingTokens)
{
}

void PreProcessorDirectiveBlock::Evaluate(PreProcessorBlockEvaluator& evaluator) const
{
	evaluator.EvaluateDirectiveBlock(*this);
}

PreProcessorBlockPtr PreProcessorDirectiveBlock::Create(
	PreProcessorDirectiveType type,
	const Position& position,
	const std::vector<PreProcessorToken>& trailingTokens)
{
	return PreProcessorBlockPtr(new PreProcessorDirectiveBlock { type, position, trailingTokens });
}

