#pragma once
#include <string>
#include "GrammarActionType.h"
#include "BinaryReader.h"
#include "BinaryWriter.h"
#include "SymbolTable.h"

class GrammarAction
{
public:
	GrammarAction() = default;
	GrammarAction(
		GrammarActionType type,
		const std::string& arg1,
		const std::string& arg2);
	GrammarAction(const GrammarAction& rhs) = default;
	~GrammarAction() = default;

	GrammarAction& operator=(const GrammarAction& rhs) = default;

	void Execute(SymbolTable& symtab) const;

	bool operator==(const GrammarAction& rhs) const;
	bool operator!=(const GrammarAction& rhs) const;
	bool operator<(const GrammarAction& rhs) const;

	std::string ToString() const;

	void Write(BinaryWriter& writer) const;
	void Read(BinaryReader& reader);

public:
	GrammarActionType type = GrammarActionType::None;
	std::string arg1;
	std::string arg2;
};

