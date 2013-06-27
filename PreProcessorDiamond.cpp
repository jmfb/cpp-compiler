#include "PreProcessorDiamond.h"

PreProcessorDiamond::PreProcessorDiamond(PreProcessorTokenStream& stream)
{
	for (;;)
	{
		auto line = stream.GetNextLine();
		if (line.tokens.empty())
			break;
		for (const auto& token: line.tokens)
			tokens.push(token);
	}
}

PreProcessorToken PreProcessorDiamond::GetNextToken()
{
	return GetNextAvailableToken();
}

