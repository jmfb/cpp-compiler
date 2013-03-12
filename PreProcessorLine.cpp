#include "PreProcessorLine.h"
#include <stdexcept>

bool PreProcessorLine::IsDirective() const
{
	for (auto& token: tokens)
	{
		if (token.IsDirectiveOperator())
			return true;
		else if (!token.CanIgnore())
			return false;
	}
	return false;
}

void PreProcessorLine::ParseDirective(
	PreProcessorToken& command,
	std::vector<PreProcessorToken>& trailingTokens) const
{
	auto foundDirectiveOperator = false;
	for (auto iter = tokens.begin(), end = tokens.end(); iter != end; ++iter)
	{
		auto& token = *iter;
		if (!foundDirectiveOperator)
		{
			if (token.IsDirectiveOperator())
			{
				command = token;
				trailingTokens.assign(iter + 1, end);
				foundDirectiveOperator = true;
			}
			else if (!token.CanIgnore())
				throw std::logic_error("Line is not a preprocessor directive (missing #).");
		}
		else
		{
			if (token.type == PreProcessorTokenType::Identifier)
			{
				command = token;
				trailingTokens.assign(iter + 1, end);
				return;
			}
			else if (!token.CanIgnore())
				throw std::logic_error("Line is not a preprocessor directive (missing command after #).");
		}
	}
}

