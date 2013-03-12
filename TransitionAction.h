#pragma once
#include <cstddef>
#include <string>
#include "TransitionType.h"
#include "Production.h"
#include "BinaryReader.h"
#include "BinaryWriter.h"

class TransitionAction
{
public:
	TransitionAction() = default;
	TransitionAction(std::size_t state);
	TransitionAction(const std::string& nonTerminal, const Production& production);
	TransitionAction(const TransitionAction& rhs) = default;
	~TransitionAction() = default;

	TransitionAction& operator=(const TransitionAction& rhs) = default;

	std::string ToString() const;

	bool operator==(const TransitionAction& rhs) const;
	bool operator!=(const TransitionAction& rhs) const;

	void Write(BinaryWriter& writer) const;
	void Read(BinaryReader& reader);
	
public:
	TransitionType type = TransitionType::Accept;
	std::size_t state = 0;
	std::string nonTerminal;
	Production production;
	std::size_t productionItemCount = 0;
};

