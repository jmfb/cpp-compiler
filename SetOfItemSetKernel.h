#pragma once
#include <vector>
#include <map>
#include <string>
#include <cstddef>
#include "ItemSetX.h"
#include "Grammar.h"

class SetOfItemSetKernel
{
public:
	SetOfItemSetKernel() = delete;
	SetOfItemSetKernel(Grammar& g, const std::string& augmentedStart);
	SetOfItemSetKernel(const SetOfItemSetKernel& rhs) = default;
	~SetOfItemSetKernel() = default;

	SetOfItemSetKernel& operator=(const SetOfItemSetKernel& rhs) = default;

private:
	bool Propogate();
	void DetermineLookAheads();
	
public:
	std::vector<ItemSetX> data;
	std::vector<std::map<std::string, std::size_t>> gotos;
	std::string augmentedStart;
	Grammar& grammar;
};

