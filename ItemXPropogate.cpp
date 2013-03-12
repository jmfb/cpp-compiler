#include "ItemXPropogate.h"
#include <sstream>

ItemXPropogate::ItemXPropogate(std::size_t state, std::size_t subItem)
	: state(state), subItem(subItem)
{
}

std::string ItemXPropogate::ToString() const
{
	std::ostringstream out;
	out << state << "(" << subItem << ")";
	return out.str();
}

bool ItemXPropogate::operator==(const ItemXPropogate& rhs) const
{
	return state == rhs.state && subItem == rhs.subItem;
}

bool ItemXPropogate::operator!=(const ItemXPropogate& rhs) const
{
	return !operator==(rhs);
}

