#pragma once
#include <set>
#include <vector>
#include <string>
#include "Macro.h"
#include "PreProcessorToken.h"
#include "MacroTableExpander.h"

class MacroExpander
{
public:
	MacroExpander() = delete;
	MacroExpander(
		const Macro& macro,
		const Position& position,
		const std::vector<std::vector<PreProcessorToken>>& parameters,
		const std::set<std::string>& visited);
	MacroExpander(const MacroExpander& rhs) = default;
	~MacroExpander() = default;

	MacroExpander& operator=(const MacroExpander& rhs) = default;

	void Expand(const MacroTableExpander& table);

	static std::vector<PreProcessorToken> EvaluateSequence(
		const MacroTableExpander& table,
		const std::vector<PreProcessorToken>& tokens,
		std::set<std::string> visited);

private:
	void StringizeParameters();
	void ConcatenateAndExpandParameters();

	static PreProcessorToken Stringize(const std::vector<PreProcessorToken>& tokens);

	static std::vector<PreProcessorToken>::iterator EvaluateObject(
		const MacroTableExpander& table,
		std::vector<PreProcessorToken>& expansion,
		std::vector<PreProcessorToken>::iterator iter,
		const std::set<std::string> visited);

	static std::vector<PreProcessorToken>::iterator EvaluateFunction(
		const MacroTableExpander& table,
		std::vector<PreProcessorToken>& expansion,
		std::vector<PreProcessorToken>::iterator iter,
		const std::set<std::string> visited);

public:
	const Macro& macro;
	Position position;
	std::vector<std::vector<PreProcessorToken>> parameters;
	std::vector<std::vector<PreProcessorToken>> expandedParameters;
	std::set<std::string> visited;
	std::vector<PreProcessorToken> expansion;
};

