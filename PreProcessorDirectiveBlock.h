#pragma once
#include <vector>
#include "Position.h"
#include "PreProcessorBlock.h"
#include "PreProcessorToken.h"
#include "PreProcessorDirectiveType.h"

class PreProcessorDirectiveBlock : public PreProcessorBlock
{
public:
	PreProcessorDirectiveBlock() = delete;
	PreProcessorDirectiveBlock(
		PreProcessorDirectiveType type,
		const Position& position,
		const std::vector<PreProcessorToken>& trailingTokens);
	PreProcessorDirectiveBlock(const PreProcessorDirectiveBlock& rhs) = delete;
	~PreProcessorDirectiveBlock() noexcept = default;

	PreProcessorDirectiveBlock& operator=(const PreProcessorDirectiveBlock& rhs) = delete;

	void Evaluate(PreProcessorBlockEvaluator& evaluator) const final;
	static PreProcessorBlockPtr Create(
		PreProcessorDirectiveType type,
		const Position& position,
		const std::vector<PreProcessorToken>& trailingTokens);

public:
	PreProcessorDirectiveType type;
	Position position;
	std::vector<PreProcessorToken> trailingTokens;
};

