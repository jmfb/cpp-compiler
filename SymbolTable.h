#pragma once
#include <vector>
#include <string>
#include <list>
#include <map>
#include <ostream>
#include "Token.h"
#include "Scope.h"

class Symbol;

class SymbolTable
{
public:
	SymbolTable();
	SymbolTable(const SymbolTable& rhs) = default;
	~SymbolTable() = default;

	SymbolTable& operator=(const SymbolTable& rhs) = default;

	void PushUnnamed();
	void PushNull();
	void PushBlock();
	void Push(const std::string& symbol);
	void Push(Symbol* symbol);
	void Pop();
	void SetQualifiedScopeGlobal();
	void SetQualifiedScope(const std::string& symbol);
	Symbol* Define(const std::string& type, const std::string& name);
	void AddAlias(const std::string& name, const std::string& symbol);
	void AddUsingNamespace(const std::string& symbol);
	void AddUsingSymbol(const std::string& symbol);
	Symbol* Lookup(const std::string& type, const std::string& name, bool isShiftOperation);
	Symbol* Define(const std::string& type, const std::string& name, const Token& token);
	void Assign(const std::string& name, Symbol* symbol);
	void Assign(const std::string& name, const Token& token);

	void WriteXml(std::ostream& out) const;

private:
	Scope* GetCurrentScope();

private:
	Scope global;
	std::vector<Scope*> scopes;
	std::list<Scope> artificialScopes;
	std::size_t nextArtificialScope = 1;
	Scope* qualified = nullptr;
	std::map<std::string, Symbol*> symbols;
	std::map<std::string, Token> tokens;
};

