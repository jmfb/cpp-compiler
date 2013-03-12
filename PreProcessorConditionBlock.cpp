#include "PreProcessorConditionBlock.h"

PreProcessorConditionBlock::PreProcessorConditionBlock(const PreProcessorCondition& firstCondition)
	: conditions({ firstCondition })
{
}

void PreProcessorConditionBlock::Evaluate(PreProcessorBlockEvaluator& evaluator) const
{
	evaluator.EvaluateConditionBlock(*this);
}

PreProcessorBlockPtr PreProcessorConditionBlock::Create(const PreProcessorCondition& firstCondition)
{
	return PreProcessorBlockPtr(new PreProcessorConditionBlock { firstCondition });
}

std::vector<PreProcessorBlockPtr> PreProcessorConditionBlock::Evaluate(const MacroTableExpander& table) const
{
	for (auto& condition: conditions)
		if (condition.Evaluate(table))
			return condition.body;
	return {};
}

