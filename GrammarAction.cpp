#include "GrammarAction.h"
#include <sstream>

GrammarAction::GrammarAction(
	GrammarActionType type,
	const std::string& arg1,
	const std::string& arg2)
	: type(type), arg1(arg1), arg2(arg2)
{
}

void GrammarAction::Execute(SymbolTable& symtab) const
{
	switch(type)
	{
	case GrammarActionType::None:
		break;
	case GrammarActionType::PushUnnamed:
		symtab.PushUnnamed();
		break;
	case GrammarActionType::PushNull:
		symtab.PushNull();
		break;
	case GrammarActionType::PushBlock:
		symtab.PushBlock();
		break;
	case GrammarActionType::Push:
		symtab.Push(arg1);
		break;
	case GrammarActionType::Pop:
		symtab.Pop();
		break;
	case GrammarActionType::ScopeGlobal:
		symtab.SetQualifiedScopeGlobal();
		break;
	case GrammarActionType::Scope:
		symtab.SetQualifiedScope(arg1);
		break;
	case GrammarActionType::Define:
		symtab.Define(arg1, arg2);
		break;
	case GrammarActionType::DefinePush:
		symtab.Push(symtab.Define(arg1, arg2));
		break;
	case GrammarActionType::Alias:
		symtab.AddAlias(arg1, arg2);
		break;
	case GrammarActionType::UsingNamespace:
		symtab.AddUsingNamespace(arg1);
		break;
	case GrammarActionType::UsingSymbol:
		symtab.AddUsingSymbol(arg1);
		break;
	}
}

bool GrammarAction::operator==(const GrammarAction& rhs) const
{
	return type == rhs.type && arg1 == rhs.arg1 && arg2 == rhs.arg2;
}

bool GrammarAction::operator!=(const GrammarAction& rhs) const
{
	return !operator==(rhs);
}

bool GrammarAction::operator<(const GrammarAction& rhs) const
{
	return	((type < rhs.type) || ((type == rhs.type) &&
			((arg1 < rhs.arg1) || ((arg1 == rhs.arg1) &&
			((arg2 < rhs.arg2)) )) ));
}

std::string GrammarAction::ToString() const
{
	std::ostringstream out;
	out << ::ToString(type) << "(" << arg1 << "," << arg2 << ")";
	return out.str();
}

void GrammarAction::Write(BinaryWriter& writer) const
{
	writer.Write(static_cast<long>(type));
	if (type != GrammarActionType::None)
	{
		writer.Write(arg1);
		writer.Write(arg2);
	}
}

void GrammarAction::Read(BinaryReader& reader)
{
	type = static_cast<GrammarActionType>(reader.ReadInt32());
	if (type != GrammarActionType::None)
	{
		arg1 = reader.ReadString();
		arg2 = reader.ReadString();
	}
}

