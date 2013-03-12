#pragma once
#include "Token.h"
#include "SentenceItem.h"

class SentenceToken : public SentenceItem
{
public:
	SentenceToken() = delete;
	SentenceToken(const Token& token);
	SentenceToken(const SentenceToken& rhs) = delete;
	~SentenceToken() noexcept = default;

	SentenceToken& operator=(const SentenceToken& rhs) = delete;

	const Position& GetPosition() const final;
	void WriteXml(std::ostream& out) const final;

	static SentenceItemPtr Create(const Token& token);

public:
	Token token;
};

