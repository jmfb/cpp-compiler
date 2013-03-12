#pragma once
#include <vector>
#include <string>
#include <cstddef>
#include "Position.h"
#include "PreProcessorToken.h"

class Macro
{
public:
	Macro() = delete;
	Macro(const std::vector<PreProcessorToken>& tokens);
	Macro(const Macro& rhs) = default;
	~Macro() = default;

	Macro& operator=(const Macro& rhs) = default;

	bool IsIdentical(const Macro& rhs) const;

	void ValidateParameterCount(std::size_t count, const Position& atPosition) const;
	bool IsParameterName(const std::string& value) const;
	std::vector<PreProcessorToken> GetParameter(
		const std::string& value,
		const std::vector<std::vector<PreProcessorToken>>& parameters) const;
	PreProcessorToken GetFirstTokenFromParameter(
		const std::string& value,
		const std::vector<std::vector<PreProcessorToken>>& parameters) const;
	PreProcessorToken GetLastTokenFromParameter(
		const std::string& value,
		const std::vector<std::vector<PreProcessorToken>>& parameters) const;

public:
	std::string name;
	Position position;
	bool isFunction = false;
	std::vector<std::string> parameterNames;
	bool isVariadic = false;
	std::vector<PreProcessorToken> replacements;
};

