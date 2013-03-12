#pragma once
#include <vector>
#include <string>
#include "Item1.h"
#include "ProductionItem.h"
#include "Grammar.h"

class ItemSet1
{
public:
	ItemSet1() = default;
	ItemSet1(const ItemSet1& rhs) = default;
	~ItemSet1() = default;

	ItemSet1& operator=(const ItemSet1& rhs) = default;

	bool AddItem(const Item1& item);
	ItemSet1 Closure(const Grammar& g) const;
	std::vector<ProductionItem> CalculateUniqueNext() const;

	std::string ToString() const;

	bool operator==(const ItemSet1& rhs) const;
	bool operator!=(const ItemSet1& rhs) const;

public:
	std::vector<Item1> items;
};

