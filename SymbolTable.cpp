#include "SymbolTable.h"
#include <stdexcept>
#include "Symbol.h"

SymbolTable::SymbolTable()
	: global(nullptr, "global::")
{
	scopes.push_back(&global);
}

void SymbolTable::PushUnnamed()
{
	artificialScopes.push_back({ GetCurrentScope(), "unnamed::" + std::to_string(nextArtificialScope++) });
	auto scope = &artificialScopes.back();
	GetCurrentScope()->AddUsing(scope);
	scopes.push_back(scope);
}

void SymbolTable::PushNull()
{
	scopes.push_back(nullptr);
}

void SymbolTable::PushBlock()
{
	artificialScopes.push_back({ GetCurrentScope(), "block::" + std::to_string(nextArtificialScope++) });
	scopes.push_back(&artificialScopes.back());
}

void SymbolTable::Push(const std::string& symbol)
{
	auto iter = symbols.find(symbol);
	if (iter == symbols.end())
		throw std::logic_error("Scope symbol not found in table.");
	scopes.push_back(&iter->second->scope);
	symbols.erase(iter);
}

void SymbolTable::Push(Symbol* symbol)
{
	scopes.push_back(&symbol->scope);
}

void SymbolTable::Pop()
{
	if (scopes.size() == 1)
		throw std::logic_error("You cannot pop the global scope.");
	scopes.pop_back();
}

void SymbolTable::SetQualifiedScopeGlobal()
{
	qualified = &global;
}

void SymbolTable::SetQualifiedScope(const std::string& symbol)
{
	auto iter = symbols.find(symbol);
	if (iter == symbols.end())
		throw std::logic_error("Symbol not defined during scope qualification.");
	qualified = &iter->second->scope;
	symbols.erase(iter);
}

Symbol* SymbolTable::Define(const std::string& type, const std::string& name)
{
	auto iter = tokens.find(name);
	if (iter == tokens.end())
		throw std::logic_error("Token not defined during symbol definition.");
	auto token = iter->second;
	tokens.erase(iter);
	return GetCurrentScope()->Define(type, token.value, token);
}

void SymbolTable::AddAlias(const std::string& name, const std::string& symbol)
{
	auto token = tokens.find(name);
	if (token == tokens.end())
		throw std::logic_error("Alias name not defined in tokens.");
	auto iter = symbols.find(symbol);
	if (iter == symbols.end())
		throw std::logic_error("Symbol for alias is not defined.");
	GetCurrentScope()->AddAlias(token->second.value, iter->second);
	tokens.erase(token);
	symbols.erase(iter);
}

void SymbolTable::AddUsingNamespace(const std::string& symbol)
{
	auto iter = symbols.find(symbol);
	if (iter == symbols.end())
		throw std::logic_error("Symbol for using is not defined.");
	GetCurrentScope()->AddUsing(&iter->second->scope);
	symbols.erase(iter);
}

void SymbolTable::AddUsingSymbol(const std::string& symbol)
{
	auto iter = symbols.find(symbol);
	if (iter == symbols.end())
		throw std::logic_error("Symbol for using directive is not defined.");
	GetCurrentScope()->AddAlias(iter->second->name, iter->second);
	//? symbols.erase(iter); ? This was missing from original version.
}

Symbol* SymbolTable::Lookup(const std::string& type, const std::string& name, bool isShiftOperation)
{
	Symbol* symbol = nullptr;
	std::vector<Scope*> visited;
	if (qualified != nullptr)
		symbol = qualified->LookupLocal(name, visited);
	else
		symbol = GetCurrentScope()->Lookup(name, visited);
	if (symbol != nullptr && symbol->type == type)
	{
		if (isShiftOperation)
			qualified = nullptr;
		return symbol;
	}
	return nullptr;
}

Symbol* SymbolTable::Define(const std::string& type, const std::string& name, const Token& token)
{
	return GetCurrentScope()->Define(type, name, token);
}

void SymbolTable::Assign(const std::string& name, Symbol* symbol)
{
	tokens.erase(name);
	auto iter = symbols.find(name);
	if (iter == symbols.end())
		symbols.insert({ name, symbol });
	else
		iter->second = symbol;
}

void SymbolTable::Assign(const std::string& name, const Token& token)
{
	symbols.erase(name);
	auto iter = tokens.find(name);
	if (iter == tokens.end())
		tokens.insert({ name, token });
	else
		iter->second = token;
}

void SymbolTable::WriteXml(std::ostream& out) const
{
	out << "<table>";
	global.WriteXml(out);
	for (auto& scope: artificialScopes)
		scope.WriteXml(out);
	out << "</table>";
}

Scope* SymbolTable::GetCurrentScope()
{
	for (auto iter = scopes.rbegin(); iter != scopes.rend(); ++iter)
	{
		if (*iter == nullptr)
			continue;
		return *iter;
	}
	return nullptr;
}

