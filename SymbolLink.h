#pragma once
#include <string>

class Symbol;

class SymbolLink
{
public:
	SymbolLink() = delete;
	SymbolLink(const std::string& name, Symbol* link);
	SymbolLink(const SymbolLink& rhs) = default;
	~SymbolLink() = default;

	SymbolLink& operator=(const SymbolLink& rhs) = default;

public:
	std::string name;
	Symbol* link;
};

