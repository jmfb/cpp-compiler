#pragma once
#include <cstddef>
#include <map>
#include <string>
#include <vector>
#include <utility>
#include "Terminal.h"
#include "TransitionAction.h"
#include "ConflictResolver.h"
#include "Token.h"
#include "SymbolTable.h"
#include "BinaryReader.h"
#include "BinaryWriter.h"

class TransitionList
{
public:
	TransitionList() = default;
	TransitionList(std::size_t index);
	TransitionList(const TransitionList& rhs) = default;
	~TransitionList() = default;

	TransitionList& operator=(const TransitionList& rhs) = default;

	void AddAction(
		const Terminal& terminal,
		const TransitionAction& action,
		ConflictResolver& resolver);
	void AddGoto(
		const std::string& nonTerminal,
		std::size_t state);
	TransitionAction GetAction(
		const Token& token,
		SymbolTable& symtab);
	std::size_t GetGoto(const std::string& nonTerminal);

	void Write(BinaryWriter& writer) const;
	void Read(BinaryReader& reader);

	std::string ToString() const;

private:
	TransitionAction ResolveMultipleTransition(
		const Token& token,
		SymbolTable& symtab,
		const std::vector<std::pair<Terminal, TransitionAction>>& actions);
	static std::string MultipleTransitionException(
		const Token& token,
		const std::vector<std::pair<Terminal, TransitionAction>>& actions);

public:
	std::size_t index = 0;
	std::map<Terminal, TransitionAction> actions;
	std::map<std::string, std::size_t> gotos;
};

