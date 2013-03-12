#pragma once
#include <vector>
#include "PreProcessorToken.h"
#include "PreProcessorBlock.h"

class PreProcessorTokenBlock : public PreProcessorBlock
{
public:
	PreProcessorTokenBlock() = delete;
	PreProcessorTokenBlock(const std::vector<PreProcessorToken>& tokens);
	PreProcessorTokenBlock(const PreProcessorTokenBlock& rhs) = delete;
	~PreProcessorTokenBlock() noexcept = default;

	PreProcessorTokenBlock& operator=(const PreProcessorTokenBlock& rhs) = delete;

	void Evaluate(PreProcessorBlockEvaluator& evaluator) const final;
	static PreProcessorBlockPtr Create(const std::vector<PreProcessorToken>& tokens);

public:
	std::vector<PreProcessorToken> tokens;
};

