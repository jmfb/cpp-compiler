#pragma once
#include <vector>
#include <iostream>
#include <string>
#include "TransitionList.h"
#include "TokenStream.h"
#include "SymbolTable.h"
#include "SentenceItem.h"
#include "BinaryReader.h"
#include "BinaryWriter.h"
#include "CodeNodeFactory.h"

class TransitionTable
{
public:
	TransitionTable() = default;
	TransitionTable(const TransitionTable& rhs) = default;
	~TransitionTable() = default;

	TransitionTable& operator=(const TransitionTable& rhs) = default;

	SentenceItemPtr Parse(TokenStream& tokens, SymbolTable& symtab, CodeNodeFactory& codeNodeFactory, std::ostream* debugDump);

	void SaveToFile(const std::string& fileName) const;
	void LoadFromFile(const std::string& fileName);

	void Write(BinaryWriter& writer) const;
	void Read(BinaryReader& reader);

	std::string ToString() const;

public:
	std::vector<TransitionList> table;
};

