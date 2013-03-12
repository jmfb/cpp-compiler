////////////////////////////////////////////////////////////////////////////////
// Filename:    TokenStream.cpp
// Description: This file implements all TokenStream member functions.
//
// Created:     2013-02-14 22:45:09
// Author:      Jacob Buysse
////////////////////////////////////////////////////////////////////////////////
#include "TokenStream.h"

TokenStream::TokenStream(PreProcessor& processor)
	: processor(processor)
{
}

Token TokenStream::GetNextToken()
{
	if (!tokens.empty())
	{
		auto token = tokens.back();
		tokens.pop_back();
		return token;
	}

	for (auto token = processor.GetNextToken();
		token.type != PreProcessorTokenType::EndOfFile;
		token = processor.GetNextToken())
	{
		if (token.CanIgnore())
			continue;
		return CreateToken(token);
	}

	return { TokenType::EndOfFile, { "", 0, 0 }, "" };
}

void TokenStream::PutBackToken(const Token& token)
{
	if (!token.IsEndOfFile())
		tokens.push_back(token);
}

