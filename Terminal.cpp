#include "Terminal.h"
#include <sstream>

const Terminal Terminal::Empty { TokenType::EndOfFile, "", {} };
const Terminal Terminal::LookAhead { TokenType::Keyword, "#", {} };

Terminal::Terminal(
	TokenType type,
	const std::string& value,
	const GrammarAction& action)
	: type(type), value(value), name(), idType(TerminalIdentifierType::Any), action(action)
{
}

Terminal::Terminal(
	TerminalIdentifierType idType,
	const std::string& type,
	const std::string& name,
	const GrammarAction& action)
	: type(TokenType::Identifier), value(type), name(name), idType(idType), action(action)
{
}

bool Terminal::Match(const Token& token, SymbolTable& symtab, bool isShiftOperation) const
{
	auto isMatch = IsMatch(token, symtab, isShiftOperation);
	if (isMatch && isShiftOperation)
		action.Execute(symtab);
	return isMatch;
}

std::string Terminal::ToString() const
{
	std::ostringstream out;
	switch(type)
	{
	case TokenType::Identifier:
		switch(idType)
		{
		case TerminalIdentifierType::Any:
			if (name.empty())
				out << "id";
			else
				out << "<id*,$" << name << ">";
			break;

		case TerminalIdentifierType::Define:
			out << "<id=" << value << ",$" << name << ">";
			break;

		case TerminalIdentifierType::Lookup:
			out << "<id:" << value << ",$" << name << ">";
			break;

		case TerminalIdentifierType::Match:
			out << "<id." << value << ">";
			break;

		default:
			out << "id";
			break;
		}
		break;

	case TokenType::Constant:
		if (value.empty())
			out << "val";
		else
			out << value;
		break;

	case TokenType::Keyword:
		if (value == "$" || value == "#")
			out << value;
		else
			out << "\"" << value << "\"";
		break;

	default:
		out << "'" << value << "'";
		break;
	}

	if (action.type != GrammarActionType::None)
		out << "=>" << action.ToString();

	return out.str();
}

bool Terminal::operator==(const Terminal& rhs) const
{
	return type == rhs.type &&
		name == rhs.name &&
		value == rhs.value &&
		idType == rhs.idType &&
		action == rhs.action;
}

bool Terminal::operator!=(const Terminal& rhs) const
{
	return !operator==(rhs);
}

bool Terminal::operator<(const Terminal& rhs) const
{
	return	((type < rhs.type) || ((type == rhs.type) &&
			((name < rhs.name) || ((name == rhs.name) &&
			((value < rhs.value) || ((value == rhs.value) &&
			((idType < rhs.idType) || ((idType == rhs.idType) &&
			((action < rhs.action)) )) )) )) ));
}

void Terminal::Write(BinaryWriter& writer) const
{
	writer.Write(static_cast<long>(type));
	writer.Write(value);
	writer.Write(name);
	writer.Write(static_cast<long>(idType));
	action.Write(writer);
}

void Terminal::Read(BinaryReader& reader)
{
	type = static_cast<TokenType>(reader.ReadInt32());
	value = reader.ReadString();
	name = reader.ReadString();
	idType = static_cast<TerminalIdentifierType>(reader.ReadInt32());
	action.Read(reader);
}

bool Terminal::IsMatch(const Token& token, SymbolTable& symtab, bool isShiftOperation) const
{
	if (type == TokenType::Keyword && value == "$")
		return token.IsEndOfFile();
	if (type != token.type)
		return false;
	if (type != TokenType::Identifier)
		return value.empty() || token.MatchValue(value);

	switch(idType)
	{
	case TerminalIdentifierType::Any:
		if (!name.empty() && isShiftOperation)
			symtab.Assign(name, token);
		return true;

	case TerminalIdentifierType::Define:
		if (isShiftOperation)
		{
			auto symbol = symtab.Define(value, token.value, token);
			if (!name.empty())
				symtab.Assign(name, symbol);
		}
		return true;

	case TerminalIdentifierType::Match:
		return token.MatchValue(value);

	case TerminalIdentifierType::Lookup:
		{
			auto symbol = symtab.Lookup(value, token.value, isShiftOperation);
			if (symbol == nullptr)
				return false;
			if (!name.empty() && isShiftOperation)
				symtab.Assign(name, symbol);
			return true;
		}
	}

	return false;
}

