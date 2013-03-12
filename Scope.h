#pragma once
#include <vector>
#include <list>
#include <string>
#include <ostream>
#include "SymbolLink.h"
#include "Token.h"

class Symbol;

class Scope
{
public:
	Scope() = delete;
	Scope(Scope* parent, const std::string& name);
	Scope(const Scope& rhs) = default;
	~Scope() = default;

	Scope& operator=(const Scope& rhs) = default;

	Symbol* LookupLocal(const std::string& name, std::vector<Scope*>& visited);
	Symbol* Lookup(const std::string& name, std::vector<Scope*>& visited);
	Symbol* Define(const std::string& type, const std::string& name, const Token& token);
	void AddAlias(const std::string& name, Symbol* link);
	void AddUsing(Scope* scope);

	void WriteXml(std::ostream& out) const;
	
	std::string ToString() const;

public:
	Scope* parent;
	std::string name;
	std::list<Symbol> symbols;
	std::vector<SymbolLink> links;
	std::vector<Scope*> usings;
};

