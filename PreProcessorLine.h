#pragma once
#include <vector>
#include "PreProcessorToken.h"

class PreProcessorLine
{
public:
	PreProcessorLine() = default;
	PreProcessorLine(const PreProcessorLine& rhs) = default;
	~PreProcessorLine() = default;

	PreProcessorLine& operator=(const PreProcessorLine& rhs) = default;

	bool IsDirective() const;
	void ParseDirective(
		PreProcessorToken& command,
		std::vector<PreProcessorToken>& trailingTokens) const;

public:
	std::vector<PreProcessorToken> tokens;
};

