#include "ProductionItem.h"
#include <sstream>

ProductionItem::ProductionItem(const Terminal& terminal)
	: isTerminal(true), isNonTerminal(false), terminal(terminal)
{
}

ProductionItem::ProductionItem(const std::string& nonTerminal)
	: isTerminal(false), isNonTerminal(true), terminal{ Terminal::Empty }, nonTerminal(nonTerminal)
{
}

bool ProductionItem::IsNull() const
{
	return !isTerminal && !isNonTerminal;
}

std::string ProductionItem::ToString() const
{
	if (isTerminal)
		return terminal.ToString();
	if (!isNonTerminal)
		return "null";
	std::ostringstream out;
	out << "<" << nonTerminal << ">";
	return out.str();
}

bool ProductionItem::operator==(const ProductionItem& rhs) const
{
	return (isTerminal && rhs.isTerminal && terminal == rhs.terminal) ||
		(isNonTerminal && rhs.isNonTerminal && nonTerminal == rhs.nonTerminal) ||
		(!isTerminal && !rhs.isTerminal && !isNonTerminal && !rhs.isNonTerminal);
}

bool ProductionItem::operator!=(const ProductionItem& rhs) const
{
	return !operator==(rhs);
}

