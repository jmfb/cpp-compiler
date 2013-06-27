#include "SentenceItem.h"
#include "SentenceToken.h"

const std::string& SentenceItem::AsToken()
{
	return As<SentenceToken>().token.value;
}

