#include "PreProcessorTokenStream.h"

PreProcessorTokenStream::PreProcessorTokenStream(CharStream& stream)
	: stream(stream)
{
}

const std::string& PreProcessorTokenStream::GetName() const
{
	return stream.GetName();
}

PreProcessorLine PreProcessorTokenStream::GetNextLine()
{
	PreProcessorLine line;
	while (!stream.IsEOF())
	{
		auto token = GetNextToken();
		if (token.type == PreProcessorTokenType::EndOfFile)
			break;
		if (token.type == PreProcessorTokenType::Newline)
		{
			if (line.tokens.empty())
				continue;
			break;
		}
		line.tokens.push_back(token);
	}
	return line;
}

