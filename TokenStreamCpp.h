////////////////////////////////////////////////////////////////////////////////
// Filename:    TokenStreamCpp.h
// Description: This file declares the TokenStreamCpp class.
//
// Created:     2013-02-14 22:52:00
// Author:      Jacob Buysse
////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "TokenStream.h"

class TokenStreamCpp : public TokenStream
{
public:
	TokenStreamCpp() = delete;
	TokenStreamCpp(PreProcessor& processor);
	TokenStreamCpp(const TokenStreamCpp& rhs) = delete;
	~TokenStreamCpp() = default;

	TokenStreamCpp& operator=(const TokenStreamCpp& rhs) = delete;

	Token CreateToken(const PreProcessorToken& ppToken) const final;

	static bool IsKeyword(const std::string& value);
	static bool IsValidOperator(const std::string& value);
};

