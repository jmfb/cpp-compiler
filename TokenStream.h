////////////////////////////////////////////////////////////////////////////////
// Filename:    TokenStream.h
// Description: This file declares the TokenStream class.
//
// Created:     2013-02-14 22:45:09
// Author:      Jacob Buysse
////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <vector>
#include "PreProcessor.h"
#include "PreProcessorToken.h"
#include "Token.h"

class TokenStream
{
public:
	TokenStream() = delete;
	TokenStream(PreProcessor& processor);
	TokenStream(const TokenStream& rhs) = delete;
	~TokenStream() = default;

	TokenStream& operator=(const TokenStream& rhs) = delete;

	Token GetNextToken();
	void PutBackToken(const Token& token);
	virtual Token CreateToken(const PreProcessorToken& ppToken) const = 0;

private:
	PreProcessor& processor;
	std::vector<Token> tokens;
};

