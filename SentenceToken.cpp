#include "SentenceToken.h"

SentenceToken::SentenceToken(const Token& token)
	: token(token)
{
}

const Position& SentenceToken::GetPosition() const
{
	return token.position;
}

void SentenceToken::WriteXml(std::ostream& out) const
{
	out << "<token>" << token.value << "</token>";
}

SentenceItemPtr SentenceToken::Create(const Token& token)
{
	return SentenceItemPtr { new SentenceToken { token } };
}

