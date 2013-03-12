#pragma once
#include <queue>
#include "PreProcessorBlockEvaluator.h"
#include "PreProcessorToken.h"
#include "PreProcessorBlock.h"

class PreProcessor : public PreProcessorBlockEvaluator
{
protected:
	PreProcessor() = default;
	PreProcessor(const PreProcessor& rhs) = delete;
	~PreProcessor() = default;

	PreProcessor& operator=(const PreProcessor& rhs) = delete;

public:
	virtual PreProcessorToken GetNextToken() = 0;

private:
	void EvaluateBlocksUntilNextTokenIsAvailable();

protected:
	PreProcessorToken GetNextAvailableToken();

protected:
	std::deque<PreProcessorBlockPtr> blocks;
	std::queue<PreProcessorToken> tokens;
};

