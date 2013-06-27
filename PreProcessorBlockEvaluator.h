#pragma once

class PreProcessorTokenBlock;
class PreProcessorDirectiveBlock;
class PreProcessorConditionBlock;

class PreProcessorBlockEvaluator
{
public:
	virtual void EvaluateTokenBlock(const PreProcessorTokenBlock& block);
	virtual void EvaluateDirectiveBlock(const PreProcessorDirectiveBlock& block);
	virtual void EvaluateConditionBlock(const PreProcessorConditionBlock& block);
};

