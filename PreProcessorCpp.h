#pragma once
#include <vector>
#include <stack>
#include <queue>
#include <set>
#include <string>
#include "MacroTable.h"
#include "PreProcessor.h"
#include "PreProcessorToken.h"
#include "PreProcessorTokenStream.h"

class PreProcessorCpp : public PreProcessor
{
public:
	PreProcessorCpp() = delete;
	PreProcessorCpp(PreProcessorTokenStream& stream);
	PreProcessorCpp(const PreProcessorCpp& rhs) = delete;
	~PreProcessorCpp() = default;

	PreProcessorCpp& operator=(const PreProcessorCpp& rhs) = delete;

	PreProcessorToken GetNextToken() final;
	PreProcessorToken GetNextTokenFromCurrentFile();
	void RestoreBlocksFromPreviousFile();
	void EvaluateTokenBlock(const PreProcessorTokenBlock& block) final;
	void EvaluateDirectiveBlock(const PreProcessorDirectiveBlock& block) final;
	void EvaluatePragmaDirective(const PreProcessorDirectiveBlock& block);
	void EvaluateIncludeDirective(const PreProcessorDirectiveBlock& block);
	void EvaluateConditionBlock(const PreProcessorConditionBlock& block) final;
	std::string ResolveFileName(
		bool systemInclude,
		const Position& position,
		const std::string& relativeFileName) const;

private:
	void ExpandMacroFunction(const PreProcessorToken& token);
	PreProcessorToken GetNextImportantToken();

private:
	MacroTable macros;
	std::vector<PreProcessorToken> expandedTokens;
	std::stack<std::vector<PreProcessorToken>> previousExpandedTokens;
	std::stack<std::deque<PreProcessorBlockPtr>> previousBlocks;
	std::stack<std::queue<PreProcessorToken>> previousTokens;
	std::stack<std::string> previousFileNames;
	std::set<std::string> pragmaOnceFileNames;
	std::string currentFileName;
};

