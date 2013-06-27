#pragma once
#include "PreProcessor.h"
#include "PreProcessorTokenStream.h"

class PreProcessorDiamond : public PreProcessor
{
public:
	PreProcessorDiamond() = delete;
	PreProcessorDiamond(PreProcessorTokenStream& stream);
	PreProcessorDiamond(const PreProcessorDiamond& rhs) = delete;
	~PreProcessorDiamond() = default;

	PreProcessorDiamond& operator=(const PreProcessorDiamond& rhs) = delete;

	PreProcessorToken GetNextToken() final;
	
};

