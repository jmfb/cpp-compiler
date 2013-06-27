#include "TransitionAction.h"
#include <sstream>

TransitionAction::TransitionAction(std::size_t state)
	: type(TransitionType::Shift), state(state)
{
}

TransitionAction::TransitionAction(const std::string& nonTerminal, const Production& production)
	: type(TransitionType::Reduce),
	nonTerminal(nonTerminal),
	production(production),
	productionString(production.ToString()),
	productionItemCount(production.items.size())
{
}

std::string TransitionAction::ToString() const
{
	std::ostringstream out;
	out << ::ToString(type);
	if (type == TransitionType::Shift)
		out << " " << state;
	else if (type == TransitionType::Reduce)
		out << " " << nonTerminal << " -> " << production.ToString();
	return out.str();
}

bool TransitionAction::operator==(const TransitionAction& rhs) const
{
	if (type != rhs.type)
		return false;
	if (type == TransitionType::Shift)
		return state == rhs.state;
	if (type == TransitionType::Reduce)
		return nonTerminal == rhs.nonTerminal && production == rhs.production;
	return true;
}

bool TransitionAction::operator!=(const TransitionAction& rhs) const
{
	return !operator==(rhs);
}

void TransitionAction::Write(BinaryWriter& writer) const
{
	writer.Write(static_cast<long>(type));
	writer.Write(state);
	writer.Write(nonTerminal);
	writer.Write(productionString);
	writer.Write(productionItemCount);
}

void TransitionAction::Read(BinaryReader& reader)
{
	type = static_cast<TransitionType>(reader.ReadInt32());
	state = reader.ReadSize();
	nonTerminal = reader.ReadString();
	productionString = reader.ReadString();
	productionItemCount = reader.ReadSize();
}

