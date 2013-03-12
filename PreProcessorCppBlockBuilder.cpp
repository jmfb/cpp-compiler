#include "PreProcessorCppBlockBuilder.h"
#include <stdexcept>
#include "PreProcessorTokenBlock.h"
#include "PreProcessorDirectiveBlock.h"

PreProcessorCppBlockBuilder::PreProcessorCppBlockBuilder(PreProcessorTokenStream& stream)
{
	for (;;)
	{
		auto line = stream.GetNextLine();
		if (line.tokens.empty())
			return;

		if (!line.IsDirective())
		{
			blocks.back().push_back(PreProcessorTokenBlock::Create(line.tokens));
			continue;
		}

		PreProcessorToken command;
		std::vector<PreProcessorToken> trailingTokens;
		line.ParseDirective(command, trailingTokens);
		ProcessDirective(command, trailingTokens);
	}
}

void PreProcessorCppBlockBuilder::GetBlocks(std::deque<PreProcessorBlockPtr>& destination) const
{
	if (!conditions.empty())
		throw std::logic_error("Missing #endif before end of file.");
	if (blocks.size() != 1)
		throw std::logic_error("Mismatched preprocessor control directives.");
	for (auto block: blocks.front())
		destination.push_back(block);
}

void PreProcessorCppBlockBuilder::ProcessDirective(
	const PreProcessorToken& command,
	const std::vector<PreProcessorToken>& trailingTokens)
{
	if (command.value == "include")
		AddDirective(PreProcessorDirectiveType::Include, command, trailingTokens);
	else if (command.value == "define")
		AddDirective(PreProcessorDirectiveType::Define, command, trailingTokens);
	else if (command.value == "undef")
		AddDirective(PreProcessorDirectiveType::Undefine, command, trailingTokens);
	else if (command.value == "line")
		AddDirective(PreProcessorDirectiveType::Line, command, trailingTokens);
	else if (command.value == "error")
		AddDirective(PreProcessorDirectiveType::Error, command, trailingTokens);
	else if (command.value == "warning")
		AddDirective(PreProcessorDirectiveType::Warning, command, trailingTokens);
	else if (command.value == "pragma")
		AddDirective(PreProcessorDirectiveType::Pragma, command, trailingTokens);
	else if (command.value == "#")
		AddDirective(PreProcessorDirectiveType::Empty, command, trailingTokens);
	else if (command.value == "ifdef")
		AddNewConditionBlock(PreProcessorConditionType::IfDefined, command, trailingTokens);
	else if (command.value == "ifndef")
		AddNewConditionBlock(PreProcessorConditionType::IfNotDefined, command, trailingTokens);
	else if (command.value == "if")
		AddNewConditionBlock(PreProcessorConditionType::If, command, trailingTokens);
	else if (command.value == "elif")
		AddElseCondition(PreProcessorConditionType::ElseIf, command, trailingTokens);
	else if (command.value == "else")
		AddElseCondition(PreProcessorConditionType::Else, command, trailingTokens);
	else if (command.value == "endif")
		FinishConditionBlock();
	else
		throw std::logic_error("Invalid preprocessor command: " + command.value);
}

void PreProcessorCppBlockBuilder::AddDirective(
	PreProcessorDirectiveType type,
	const PreProcessorToken& command,
	const std::vector<PreProcessorToken>& trailingTokens)
{
	blocks.back().push_back(PreProcessorDirectiveBlock::Create(type, command.position, trailingTokens));
}

void PreProcessorCppBlockBuilder::AddNewConditionBlock(
	PreProcessorConditionType type,
	const PreProcessorToken& command,
	const std::vector<PreProcessorToken>& trailingTokens)
{
	conditions.push_back(PreProcessorConditionBlock::Create({ type, command.position, trailingTokens }));
	blocks.push_back({});
}

void PreProcessorCppBlockBuilder::AddElseCondition(
	PreProcessorConditionType type,
	const PreProcessorToken& command,
	const std::vector<PreProcessorToken>& trailingTokens)
{
	if (conditions.empty())
		throw std::logic_error("#elif/else encountered without corresponding #if/def/ndef.");
	auto currentConditionBlock = GetCurrentConditionBlock();
	if (currentConditionBlock->conditions.back().type == PreProcessorConditionType::Else)
		throw std::logic_error("#elif/else encountered after #else.");
	currentConditionBlock->conditions.back().body = blocks.back();
	blocks.back().clear();
	currentConditionBlock->conditions.push_back({ type, command.position, trailingTokens });
}

void PreProcessorCppBlockBuilder::FinishConditionBlock()
{
	if (conditions.empty())
		throw std::logic_error("#endif encountered wihtout corresponding #if/def/ndef.");
	GetCurrentConditionBlock()->conditions.back().body = blocks.back();
	blocks.pop_back();
	blocks.back().push_back(conditions.back());
	conditions.pop_back();
}

PreProcessorConditionBlock* PreProcessorCppBlockBuilder::GetCurrentConditionBlock()
{
	if (conditions.empty())
		throw std::logic_error("There is no current condition.");
	return dynamic_cast<PreProcessorConditionBlock*>(conditions.back().get());
}

