#include "GrammarActionType.h"
#include <stdexcept>

std::string ToString(GrammarActionType type)
{
	switch(type)
	{
	case GrammarActionType::None:
		return "None";
	case GrammarActionType::PushUnnamed:
		return "PushUnnamed";
	case GrammarActionType::PushNull:
		return "PushNull";
	case GrammarActionType::PushBlock:
		return "PushBlock";
	case GrammarActionType::Push:
		return "Push";
	case GrammarActionType::Pop:
		return "Pop";
	case GrammarActionType::ScopeGlobal:
		return "ScopeGlobal";
	case GrammarActionType::Scope:
		return "Scope";
	case GrammarActionType::Define:
		return "Define";
	case GrammarActionType::DefinePush:
		return "DefinePush";
	case GrammarActionType::Alias:
		return "Alias";
	case GrammarActionType::UsingNamespace:
		return "UsingNamespace";
	case GrammarActionType::UsingSymbol:
		return "UsingSymbol";
	}
	throw std::logic_error("Invalid grammar aciton type.");
}

