#include "PreProcessorCpp.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <fstream>
#include "PreProcessorCppBlockBuilder.h"
#include "PreProcessorTokenBlock.h"
#include "PreProcessorDirectiveBlock.h"
#include "PreProcessorConditionBlock.h"
#include "MacroExpander.h"
#include "CharStreamCpp.h"
#include "PreProcessorTokenStreamCpp.h"

PreProcessorCpp::PreProcessorCpp(PreProcessorTokenStream& stream)
{
	macros.Define({
		{ PreProcessorTokenType::Identifier, {"", 0, 0}, "__cplusplus" },
		{ PreProcessorTokenType::Whitespace, {"", 0, 0}, " " },
		{ PreProcessorTokenType::Number, {"", 0, 0}, "201103L" } //199711L for pre c++11
	});
	macros.Define({
		{ PreProcessorTokenType::Identifier, {"", 0, 0}, "_WIN32" },
		{ PreProcessorTokenType::Whitespace, {"", 0, 0}, " " },
		{ PreProcessorTokenType::Number, {"", 0, 0}, "1" }
	});
	macros.Define({
		{ PreProcessorTokenType::Identifier, {"", 0, 0}, "__GNUC__" },
		{ PreProcessorTokenType::Whitespace, {"", 0, 0}, " " },
		{ PreProcessorTokenType::Number, {"", 0, 0}, "4" }
	});
	macros.Define({
		{ PreProcessorTokenType::Identifier, {"", 0, 0}, "__CHAR_BIT__" },
		{ PreProcessorTokenType::Whitespace, {"", 0, 0}, " " },
		{ PreProcessorTokenType::Number, {"", 0, 0}, "8" }
	});
	macros.Define({
		{ PreProcessorTokenType::Identifier, {"", 0, 0}, "__LDBL_DIG__" },
		{ PreProcessorTokenType::Whitespace, {"", 0, 0}, " " },
		{ PreProcessorTokenType::Number, {"", 0, 0}, "18" }
	});
	macros.Define({
		{ PreProcessorTokenType::Identifier, {"", 0, 0}, "__DBL_DIG__" },
		{ PreProcessorTokenType::Whitespace, {"", 0, 0}, " " },
		{ PreProcessorTokenType::Number, {"", 0, 0}, "15" }
	});
	macros.Define({
		{ PreProcessorTokenType::Identifier, {"", 0, 0}, "__FLT_DIG__" },
		{ PreProcessorTokenType::Whitespace, {"", 0, 0}, " " },
		{ PreProcessorTokenType::Number, {"", 0, 0}, "6" }
	});
	macros.Define({
		{ PreProcessorTokenType::Identifier, {"", 0, 0}, "__LDBL_MANT_DIG__" },
		{ PreProcessorTokenType::Whitespace, {"", 0, 0}, " " },
		{ PreProcessorTokenType::Number, {"", 0, 0}, "64" }
	});
	macros.Define({
		{ PreProcessorTokenType::Identifier, {"", 0, 0}, "__DBL_MANT_DIG__" },
		{ PreProcessorTokenType::Whitespace, {"", 0, 0}, " " },
		{ PreProcessorTokenType::Number, {"", 0, 0}, "53" }
	});
	macros.Define({
		{ PreProcessorTokenType::Identifier, {"", 0, 0}, "__FLT_MANT_DIG__" },
		{ PreProcessorTokenType::Whitespace, {"", 0, 0}, " " },
		{ PreProcessorTokenType::Number, {"", 0, 0}, "24" }
	});
	macros.Define({
		{ PreProcessorTokenType::Identifier, {"", 0, 0}, "__LDBL_MAX_10_EXP__" },
		{ PreProcessorTokenType::Whitespace, {"", 0, 0}, " " },
		{ PreProcessorTokenType::Number, {"", 0, 0}, "4932" }
	});
	macros.Define({
		{ PreProcessorTokenType::Identifier, {"", 0, 0}, "__DBL_MAX_10_EXP__" },
		{ PreProcessorTokenType::Whitespace, {"", 0, 0}, " " },
		{ PreProcessorTokenType::Number, {"", 0, 0}, "308" }
	});
	macros.Define({
		{ PreProcessorTokenType::Identifier, {"", 0, 0}, "__FLT_MAX_10_EXP__" },
		{ PreProcessorTokenType::Whitespace, {"", 0, 0}, " " },
		{ PreProcessorTokenType::Number, {"", 0, 0}, "38" }
	});
	macros.Define({
		{ PreProcessorTokenType::Identifier, {"", 0, 0}, "__SIZE_TYPE__" },
		{ PreProcessorTokenType::Whitespace, {"", 0, 0}, " " },
		{ PreProcessorTokenType::Identifier, {"", 0, 0}, "long" },
		{ PreProcessorTokenType::Whitespace, {"", 0, 0}, " " },
		{ PreProcessorTokenType::Identifier, {"", 0, 0}, "long" },
		{ PreProcessorTokenType::Whitespace, {"", 0, 0}, " " },
		{ PreProcessorTokenType::Identifier, {"", 0, 0}, "unsigned" },
		{ PreProcessorTokenType::Whitespace, {"", 0, 0}, " " },
		{ PreProcessorTokenType::Identifier, {"", 0, 0}, "int" }
	});
	macros.Define({
		{ PreProcessorTokenType::Identifier, {"", 0, 0}, "__PTRDIFF_TYPE__" },
		{ PreProcessorTokenType::Whitespace, {"", 0, 0}, " " },
		{ PreProcessorTokenType::Identifier, {"", 0, 0}, "long" },
		{ PreProcessorTokenType::Whitespace, {"", 0, 0}, " " },
		{ PreProcessorTokenType::Identifier, {"", 0, 0}, "long" },
		{ PreProcessorTokenType::Whitespace, {"", 0, 0}, " " },
		{ PreProcessorTokenType::Identifier, {"", 0, 0}, "int" }
	});

	currentFileName = stream.GetName();
	PreProcessorCppBlockBuilder builder { stream };
	builder.GetBlocks(blocks);
}

PreProcessorToken PreProcessorCpp::GetNextToken()
{
	for (;;)
	{
		auto token = GetNextTokenFromCurrentFile();
		if (token.type != PreProcessorTokenType::EndOfFile)
			return token;
		if (previousBlocks.empty())
			return token;
		RestoreBlocksFromPreviousFile();
	}
}

void PreProcessorCpp::RestoreBlocksFromPreviousFile()
{
	if (previousBlocks.empty())
	{
		blocks = {};
		tokens = {};
		expandedTokens = {};
		currentFileName.clear();
	}
	else
	{
		blocks = previousBlocks.top();
		tokens = previousTokens.top();
		expandedTokens = previousExpandedTokens.top();
		currentFileName = previousFileNames.top();

		previousBlocks.pop();
		previousTokens.pop();
		previousExpandedTokens.pop();
		previousFileNames.pop();
	}
}

PreProcessorToken PreProcessorCpp::GetNextTokenFromCurrentFile()
{
	for (;;)
	{
		if (!expandedTokens.empty())
		{
			auto token = expandedTokens.back();
			expandedTokens.pop_back();
			return token;
		}

		auto token = GetNextAvailableToken();
		if (token.type != PreProcessorTokenType::Identifier)
			return token;

		auto type = macros.GetMacroType(token.value);
		switch(type)
		{
		case MacroType::Undefined:
			return token;

		case MacroType::Object:
			expandedTokens = macros.Expand(token, {}, {});
			std::reverse(expandedTokens.begin(), expandedTokens.end());
			break;

		case MacroType::Function:
			ExpandMacroFunction(token);
			std::reverse(expandedTokens.begin(), expandedTokens.end());
			break;
		}
	}
}

void PreProcessorCpp::EvaluateTokenBlock(const PreProcessorTokenBlock& block)
{
	for (auto& token: block.tokens)
		tokens.push(token);
}

void PreProcessorCpp::EvaluateDirectiveBlock(const PreProcessorDirectiveBlock& block)
{
	switch(block.type)
	{
	case PreProcessorDirectiveType::Define:
		macros.Define(block.trailingTokens);
		break;
	case PreProcessorDirectiveType::Undefine:
		macros.Undefine(block.trailingTokens);
		break;
	case PreProcessorDirectiveType::Line:
		//ignore - for the purpose of this I want the real line and file names.
		break;
	case PreProcessorDirectiveType::Error:
		{
			std::ostringstream out;
			for (auto& token: block.trailingTokens)
				out << token.value;
			out << std::endl << block.position.ToString();
			throw std::logic_error(out.str());
		}
		break;
	case PreProcessorDirectiveType::Warning:
		std::cout << "WARNING: ";
		for (auto& token: block.trailingTokens)
			std::cout << token.value;
		std::cout << std::endl;
		break;
	case PreProcessorDirectiveType::Pragma:
		EvaluatePragmaDirective(block);
		break;
	case PreProcessorDirectiveType::Include:
		EvaluateIncludeDirective(block);
		break;
	case PreProcessorDirectiveType::Empty:
		break;
	}
}

void PreProcessorCpp::EvaluatePragmaDirective(const PreProcessorDirectiveBlock& block)
{
	auto iter = block.trailingTokens.begin();
	while (iter != block.trailingTokens.end() && iter->CanIgnore())
		++iter;
	if (iter != block.trailingTokens.end() &&
		iter->type == PreProcessorTokenType::Identifier &&
		iter->value == "once")
	{
		if (pragmaOnceFileNames.find(currentFileName) == pragmaOnceFileNames.end())
			pragmaOnceFileNames.insert(currentFileName);
		else
			RestoreBlocksFromPreviousFile();
	}
}

void PreProcessorCpp::EvaluateIncludeDirective(const PreProcessorDirectiveBlock& block)
{
	auto includeTokens = block.trailingTokens;

	std::string fileName;
	auto systemInclude = false;
	for (auto first = true; ; first = false)
	{
		auto begin = includeTokens.begin();
		while (begin != includeTokens.end() && begin->CanIgnore())
			++begin;
		if (begin == includeTokens.end())
			throw std::logic_error("#include must be followed by tokens.");

		if (begin->type == PreProcessorTokenType::String)
			fileName = begin->value.substr(1, begin->value.size() - 2);
		else if (begin->IsOperator("<"))
		{
			systemInclude = true;
			std::ostringstream out;
			auto iter = begin + 1;
			for (; iter != includeTokens.end() && !iter->IsOperator(">"); ++iter)
				out << iter->value;
			if (iter == includeTokens.end())
				throw std::logic_error("#include <... was missing trailing >.");
			fileName = out.str();
		}
		else if (first)
		{
			includeTokens = MacroExpander::EvaluateSequence(macros, block.trailingTokens, {});
			continue;
		}
		else
			throw std::logic_error("#include must be followed by \"...\" or <...>.");
		break;
	}

	fileName = ResolveFileName(systemInclude, block.position, fileName);

	previousBlocks.push(blocks);
	previousTokens.push(tokens);
	previousExpandedTokens.push(expandedTokens);
	previousFileNames.push(currentFileName);
	blocks = {};
	tokens = {};
	expandedTokens = {};
	currentFileName = fileName;

	std::ifstream in { fileName };
	CharStreamCpp stream { in, fileName };
	PreProcessorTokenStreamCpp ppTokenStream { stream };
	PreProcessorCppBlockBuilder builder { ppTokenStream };
	builder.GetBlocks(blocks);
}

void PreProcessorCpp::EvaluateConditionBlock(const PreProcessorConditionBlock& block)
{
	auto result = block.Evaluate(macros);
	blocks.insert(blocks.begin(), result.begin(), result.end());
}

void PreProcessorCpp::ExpandMacroFunction(const PreProcessorToken& token)
{
	enum class State
	{
		Initial,
		Parameter,
		Comma
	};

	auto lparen = GetNextImportantToken();
	if (lparen.value != "(")
		throw std::logic_error("Function-like macro expansion requires argument list.");

	std::vector<std::vector<PreProcessorToken>> parameters;
	std::vector<PreProcessorToken> nextParameter;
	auto unclosedParens = 0ul;

	auto state = State::Initial;
	for (auto done = false; !done; )
	{
		auto token = GetNextAvailableToken();
		if (token.type == PreProcessorTokenType::EndOfFile)
			throw std::logic_error("End of file encountered during function-like macro expansion.");
		switch(state)
		{
		case State::Initial:
			if (!token.CanIgnore())
			{
				if (token.value == ")")
				{
					done = true;
				}
				else if (token.value == ",")
				{
					state = State::Comma;
					parameters.push_back({});
				}
				else
				{
					if (token.value == "(")
						++unclosedParens;
					nextParameter.push_back(token);
					state = State::Parameter;
				}
			}
			break;

		case State::Parameter:
			if (token.value == "(")
			{
				++unclosedParens;
				nextParameter.push_back(token);
			}
			else if (token.value == ")")
			{
				if (unclosedParens == 0)
				{
					parameters.push_back(nextParameter);
					done = true;
				}
				else
				{
					nextParameter.push_back(token);
					--unclosedParens;
				}
			}
			else if (token.value == "," && unclosedParens == 0)
			{
				parameters.push_back(nextParameter);
				nextParameter.clear();
				state = State::Comma;
			}
			else
			{
				nextParameter.push_back(token);
			}
			break;

		case State::Comma:
			if (!token.CanIgnore())
			{
				if (token.value == ")")
				{
					parameters.push_back({});
					done = true;
				}
				else if (token.value == ",")
				{
					parameters.push_back({});
				}
				else
				{
					if (token.value == "(")
						++unclosedParens;
					nextParameter.push_back(token);
					state = State::Parameter;
				}
			}
			break;
		}
	}

	expandedTokens = macros.Expand(token, parameters, {});
}

PreProcessorToken PreProcessorCpp::GetNextImportantToken()
{
	for (;;)
	{
		auto token = GetNextAvailableToken();
		if (!token.CanIgnore())
			return token;
	}
}

