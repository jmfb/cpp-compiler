#pragma once
#include <string>
#include "Position.h"
#include "PreProcessorTokenType.h"

class PreProcessorToken
{
public:
	PreProcessorToken() = default;
	PreProcessorToken(
		PreProcessorTokenType type,
		const Position& position,
		const std::string& value);
	PreProcessorToken(const PreProcessorToken& rhs) = default;
	~PreProcessorToken() = default;

	PreProcessorToken& operator=(const PreProcessorToken& rhs) = default;

	bool IsDirectiveOperator() const;
	bool IsOperator(const std::string& op) const;
	bool CanIgnore() const;
	bool operator==(const PreProcessorToken& rhs) const;
	std::string Stringize() const;
	PreProcessorToken Concatenate(const PreProcessorToken& rhs) const;

	std::string ToString() const;

public:
	PreProcessorTokenType type = PreProcessorTokenType::EndOfFile;
	Position position;
	std::string value;
};

