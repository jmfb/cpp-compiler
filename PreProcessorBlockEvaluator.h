#pragma once

class PreProcessorTokenBlock;
class PreProcessorDirectiveBlock;
class PreProcessorConditionBlock;

class PreProcessorBlockEvaluator
{
public:
	virtual void EvaluateTokenBlock(const PreProcessorTokenBlock& block) = 0;
	virtual void EvaluateDirectiveBlock(const PreProcessorDirectiveBlock& block) = 0;
	virtual void EvaluateConditionBlock(const PreProcessorConditionBlock& block) = 0;
};

