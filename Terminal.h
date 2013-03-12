#pragma once
#include <string>
#include "TokenType.h"
#include "TerminalIdentifierType.h"
#include "GrammarAction.h"
#include "BinaryReader.h"
#include "BinaryWriter.h"
#include "SymbolTable.h"
#include "Token.h"

class Terminal
{
public:
	Terminal() = default;
	Terminal(
		TokenType type,
		const std::string& value,
		const GrammarAction& action);
	Terminal(
		TerminalIdentifierType idType,
		const std::string& type,
		const std::string& name,
		const GrammarAction& action);
	Terminal(const Terminal& rhs) = default;
	~Terminal() = default;

	Terminal& operator=(const Terminal& rhs) = default;

	bool Match(const Token& token, SymbolTable& symtab, bool isShiftOperation) const;

	std::string ToString() const;

	bool operator==(const Terminal& rhs) const;
	bool operator!=(const Terminal& rhs) const;
	bool operator<(const Terminal& rhs) const;

	void Write(BinaryWriter& writer) const;
	void Read(BinaryReader& reader);

	static const Terminal Empty;
	static const Terminal LookAhead;

private:
	bool IsMatch(const Token& token, SymbolTable& symtab, bool isShiftOperation) const;

public:
	TokenType type = TokenType::EndOfFile;
	std::string value;
	std::string name;
	TerminalIdentifierType idType = TerminalIdentifierType::Any;
	GrammarAction action;
};

