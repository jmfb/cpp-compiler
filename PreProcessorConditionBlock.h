#pragma once
#include <vector>
#include "PreProcessorCondition.h"
#include "PreProcessorBlock.h"
#include "MacroTableExpander.h"

class PreProcessorConditionBlock : public PreProcessorBlock
{
public:
	PreProcessorConditionBlock() = delete;
	PreProcessorConditionBlock(const PreProcessorCondition& firstCondition);
	PreProcessorConditionBlock(const PreProcessorConditionBlock& rhs) = delete;
	~PreProcessorConditionBlock() noexcept = default;

	PreProcessorConditionBlock& operator=(const PreProcessorConditionBlock& rhs) = delete;

	void Evaluate(PreProcessorBlockEvaluator& evaluator) const final;
	static PreProcessorBlockPtr Create(const PreProcessorCondition& firstCondition);

	std::vector<PreProcessorBlockPtr> Evaluate(const MacroTableExpander& table) const;
	
public:
	std::vector<PreProcessorCondition> conditions;
};

