#pragma once
#include <vector>
#include <string>
#include "Terminal.h"
#include "ProductionItem.h"

class Production
{
public:
	Production() = default;
	Production(const Production& rhs) = default;
	~Production() = default;

	Production& operator=(const Production& rhs) = default;

	Production& AddItem(const Terminal& terminal);
	Production& AddItem(const std::string& nonTerminal);
	Production& AddItem(const ProductionItem& item);
	Production& RemoveFront();

	void AddFirst(const Terminal& terminal);
	void MergeFirst(const std::vector<Terminal>& terminals);

	std::string ToString() const;

	bool operator==(const Production& rhs) const;
	bool operator!=(const Production& rhs) const;

public:
	std::vector<ProductionItem> items;
	std::vector<Terminal> first;
};

