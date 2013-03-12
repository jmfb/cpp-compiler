#include "PreProcessor.h"

void PreProcessor::EvaluateBlocksUntilNextTokenIsAvailable()
{
	while (tokens.empty() && !blocks.empty())
	{
		auto block = blocks.front();
		blocks.pop_front();
		block->Evaluate(*this);
	}
}

PreProcessorToken PreProcessor::GetNextAvailableToken()
{
	EvaluateBlocksUntilNextTokenIsAvailable();
	if (tokens.empty())
		return { PreProcessorTokenType::EndOfFile, { "", 0, 0 }, "" };
	auto token = tokens.front();
	tokens.pop();
	return token;
}

