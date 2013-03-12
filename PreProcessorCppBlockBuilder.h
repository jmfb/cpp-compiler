#pragma once
#include <vector>
#include <deque>
#include "PreProcessorToken.h"
#include "PreProcessorBlock.h"
#include "PreProcessorDirectiveType.h"
#include "PreProcessorConditionType.h"
#include "PreProcessorConditionBlock.h"
#include "PreProcessorTokenStream.h"

class PreProcessorCppBlockBuilder
{
public:
	PreProcessorCppBlockBuilder() = delete;
	PreProcessorCppBlockBuilder(PreProcessorTokenStream& stream);
	PreProcessorCppBlockBuilder(const PreProcessorCppBlockBuilder& rhs) = delete;
	~PreProcessorCppBlockBuilder() = default;

	PreProcessorCppBlockBuilder& operator=(const PreProcessorCppBlockBuilder& rhs) = delete;

	void GetBlocks(std::deque<PreProcessorBlockPtr>& destination) const;

private:
	void ProcessDirective(
		const PreProcessorToken& command,
		const std::vector<PreProcessorToken>& trailingTokens);
	void AddDirective(
		PreProcessorDirectiveType type,
		const PreProcessorToken& command,
		const std::vector<PreProcessorToken>& trailingTokens);
	void AddNewConditionBlock(
		PreProcessorConditionType type,
		const PreProcessorToken& command,
		const std::vector<PreProcessorToken>& trailingTokens);
	void AddElseCondition(
		PreProcessorConditionType type,
		const PreProcessorToken& command,
		const std::vector<PreProcessorToken>& trailingTokens);
	void FinishConditionBlock();
	PreProcessorConditionBlock* GetCurrentConditionBlock();

private:
	std::vector<std::vector<PreProcessorBlockPtr>> blocks { {} };
	std::vector<PreProcessorBlockPtr> conditions;
};

