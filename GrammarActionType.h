#pragma once
#include <string>

enum class GrammarActionType
{
	None,
	PushUnnamed,
	PushNull,
	PushBlock,
	Push,
	Pop,
	ScopeGlobal,
	Scope,
	Define,
	DefinePush,
	Alias,
	UsingNamespace,
	UsingSymbol
};

std::string ToString(GrammarActionType type);

