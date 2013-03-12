#pragma once
#include <string>
#include <cstddef>
#include <vector>
#include "Production.h"
#include "Terminal.h"
#include "ItemXPropogate.h"

class ItemX
{
public:
	ItemX() = delete;
	ItemX(
		const std::string& nonTerminal,
		const Production& production,
		std::size_t index);
	ItemX(const ItemX& rhs) = default;
	~ItemX() = default;

	ItemX& operator=(const ItemX& rhs) = default;

	bool HasNonTerminalSuffix(std::string& suffix) const;
	bool IsNext(const ProductionItem& item) const;
	ProductionItem GetNext() const;
	bool AddLookAhead(const Terminal& terminal);
	void AddPropogate(std::size_t state, std::size_t subItem);

	std::string ToString() const;

	bool operator==(const ItemX& rhs) const;
	bool operator!=(const ItemX& rhs) const;

public:
	std::string nonTerminal;
	Production production;
	std::size_t index;
	std::vector<Terminal> lookAhead;
	std::vector<ItemXPropogate> propogate;
};

