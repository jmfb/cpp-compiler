#pragma once
#include <string>
#include "Terminal.h"

class ProductionItem
{
public:
	ProductionItem() = default;
	ProductionItem(const Terminal& terminal);
	ProductionItem(const std::string& nonTerminal);
	ProductionItem(const ProductionItem& rhs) = default;
	~ProductionItem() = default;

	ProductionItem& operator=(const ProductionItem& rhs) = default;

	bool IsNull() const;
	
	std::string ToString() const;

	bool operator==(const ProductionItem& rhs) const;
	bool operator!=(const ProductionItem& rhs) const;

public:
	bool isTerminal = false;
	bool isNonTerminal = false;
	Terminal terminal;
	std::string nonTerminal;
};

