#pragma once
#include "CharStream.h"
#include "PreProcessorLine.h"
#include "PreProcessorToken.h"

class PreProcessorTokenStream
{
public:
	PreProcessorTokenStream() = delete;
	PreProcessorTokenStream(CharStream& stream);
	PreProcessorTokenStream(const PreProcessorTokenStream& rhs) = delete;
	~PreProcessorTokenStream() = default;

	PreProcessorTokenStream& operator=(const PreProcessorTokenStream& rhs) = delete;

	const std::string& GetName() const;
	PreProcessorLine GetNextLine();
	virtual PreProcessorToken GetNextToken() = 0;

protected:
	CharStream& stream;
};

