#include "SentenceItem.h"
#include "SentenceToken.h"
#include "Sentence.h"

const std::string& SentenceItem::AsToken()
{
	return As<SentenceToken>().token.value;
}

CodeNodePtr SentenceItem::AsNode()
{
	return As<Sentence>().codeNode;
}

