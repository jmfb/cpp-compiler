#include "PreProcessorTokenBlock.h"

PreProcessorTokenBlock::PreProcessorTokenBlock(const std::vector<PreProcessorToken>& tokens)
	: tokens(tokens)
{
}

void PreProcessorTokenBlock::Evaluate(PreProcessorBlockEvaluator& evaluator) const
{
	evaluator.EvaluateTokenBlock(*this);
}

PreProcessorBlockPtr PreProcessorTokenBlock::Create(const std::vector<PreProcessorToken>& tokens)
{
	return PreProcessorBlockPtr(new PreProcessorTokenBlock { tokens });
}

