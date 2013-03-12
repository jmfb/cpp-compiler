#pragma once
#include <cstddef>
#include <string>

class ItemXPropogate
{
public:
	ItemXPropogate() = delete;
	ItemXPropogate(std::size_t state, std::size_t subItem);
	ItemXPropogate(const ItemXPropogate& rhs) = default;
	~ItemXPropogate() = default;

	ItemXPropogate& operator=(const ItemXPropogate& rhs) = default;

	std::string ToString() const;

	bool operator==(const ItemXPropogate& rhs) const;
	bool operator!=(const ItemXPropogate& rhs) const;
	
public:
	std::size_t state;
	std::size_t subItem;
};

