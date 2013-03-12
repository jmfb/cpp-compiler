#pragma once
#include <vector>
#include <string>
#include <cstddef>
#include "ItemX.h"
#include "ProductionItem.h"
#include "Grammar.h"

class ItemSetX
{
public:
	ItemSetX() = default;
	ItemSetX(const ItemSetX& rhs) = default;
	~ItemSetX() = default;

	ItemSetX& operator=(const ItemSetX& rhs) = default;

	bool AddItem(const ItemX& item);
	ItemSetX Closure(const Grammar& g) const;
	ItemSetX Goto(const Grammar& g, const ProductionItem& pitem) const;
	ItemSetX GotoRaw(const ProductionItem& pitem) const;
	std::vector<ProductionItem> CalculateUniqueNext() const;
	std::size_t FindSubItem(const Production& production, std::size_t dot) const;

	std::string ToString() const;
	
	bool operator==(const ItemSetX& rhs) const;
	bool operator!=(const ItemSetX& rhs) const;
	
public:
	std::vector<ItemX> items;
};

