#pragma once
#include <string>
#include <cstddef>
#include "Production.h"
#include "ProductionItem.h"
#include "Terminal.h"

class Item1
{
public:
	Item1() = delete;
	Item1(
		const std::string& nonTerminal,
		const Production& production,
		std::size_t index,
		const Terminal& follow);
	Item1(const Item1& rhs) = default;
	~Item1() = default;

	Item1& operator=(const Item1& rhs) = default;

	bool HasNonTerminalSuffix(std::string& suffix) const;
	bool IsNext(const ProductionItem& item) const;
	ProductionItem GetNext() const;

	std::string ToString() const;

	bool operator==(const Item1& rhs) const;
	bool operator!=(const Item1& rhs) const;

public:
	std::string nonTerminal;
	Production production;
	std::size_t index;
	Terminal follow;
};

