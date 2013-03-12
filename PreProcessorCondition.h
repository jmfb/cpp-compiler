#pragma once
#include <vector>
#include "Position.h"
#include "PreProcessorConditionType.h"
#include "PreProcessorToken.h"
#include "PreProcessorBlock.h"
#include "MacroTableExpander.h"

class PreProcessorCondition
{
public:
	PreProcessorCondition() = delete;
	PreProcessorCondition(
		PreProcessorConditionType type,
		const Position& position,
		const std::vector<PreProcessorToken>& expression);
	PreProcessorCondition(const PreProcessorCondition& rhs) = default;
	~PreProcessorCondition() = default;

	PreProcessorCondition& operator=(const PreProcessorCondition& rhs) = default;

	bool Evaluate(const MacroTableExpander& table) const;

private:
	bool EvaluateCondition(const MacroTableExpander& table) const;
	bool EvaluateDefined(const MacroTableExpander& table) const;

public:
	PreProcessorConditionType type;
	Position position;
	std::vector<PreProcessorToken> expression;
	std::vector<PreProcessorBlockPtr> body;
};

