#pragma once
#include "TokenStream.h"

class TokenStreamDiamond : public TokenStream
{
public:
	TokenStreamDiamond() = delete;
	TokenStreamDiamond(PreProcessor& processor);
	TokenStreamDiamond(const TokenStreamDiamond& rhs) = default;
	~TokenStreamDiamond() = default;

	TokenStreamDiamond& operator=(const TokenStreamDiamond& rhs) = default;

	Token CreateToken(const PreProcessorToken& ppToken) const final;

	static bool IsKeyword(const std::string& value);
	static bool IsValidOperator(const std::string& value);
};

