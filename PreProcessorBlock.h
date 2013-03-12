#pragma once
#include <memory>
#include "PreProcessorBlockEvaluator.h"

class PreProcessorBlock
{
public:
	virtual ~PreProcessorBlock() = default;
	virtual void Evaluate(PreProcessorBlockEvaluator& evaluator) const = 0;
};

typedef std::shared_ptr<PreProcessorBlock> PreProcessorBlockPtr;

