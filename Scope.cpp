#include "Scope.h"
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include "Symbol.h"

Scope::Scope(Scope* parent, const std::string& name)
	: parent(parent), name(name)
{
}

Symbol* Scope::LookupLocal(const std::string& name, std::vector<Scope*>& visited)
{
	for (auto& symbol: symbols)
		if (symbol.name == name)
			return &symbol;
	for (auto& link: links)
		if (link.name == name)
			return link.link;
	visited.push_back(this);
	for (auto scope: usings)
	{
		auto iter = std::find(visited.begin(), visited.end(), scope);
		if (iter == visited.end())
		{
			auto symbol = scope->LookupLocal(name, visited);
			if (symbol != nullptr)
				return symbol;
		}
	}
	return nullptr;
}

Symbol* Scope::Lookup(const std::string& name, std::vector<Scope*>& visited)
{
	auto symbol = LookupLocal(name, visited);
	if (symbol == nullptr && parent != nullptr)
		symbol = parent->Lookup(name, visited);
	return symbol;
}

Symbol* Scope::Define(const std::string& type, const std::string& name, const Token& token)
{
	std::vector<Scope*> visited;
	auto symbol = LookupLocal(name, visited);
	if (symbol == nullptr)
	{
		symbols.push_back({ type, name, this });
		symbols.back().definitions.push_back(token);
		symbol = &symbols.back();
	}
	else
	{
		if (symbol->type != type)
			throw std::logic_error("Redefinition of identifier with different types.");
		symbol->definitions.push_back(token);
	}
	return symbol;
}

void Scope::AddAlias(const std::string& name, Symbol* link)
{
	auto iter = std::find_if(links.begin(), links.end(),
		[&](const SymbolLink& link){ return link.name == name; });
	if (iter == links.end())
		links.push_back({ name, link });
	else if (iter->link != link)
		throw std::logic_error("Alias already defined for another symbol.");
}

void Scope::AddUsing(Scope* scope)
{
	auto iter = std::find(usings.begin(), usings.end(), scope);
	if (iter == usings.end())
		usings.push_back(scope);
}

void Scope::WriteXml(std::ostream& out) const
{
	out << "<scope name=\"" << name << "\">";
	out << "<symbols>";
	for (auto& symbol: symbols)
		symbol.WriteXml(out);
	out << "</symbols>";
	out << "<links>";
	for (auto& link: links)
		out << "<link name=\"" << link.name << "\">" << link.link->name << "</link>";
	out << "</links>";
	out << "<usings>";
	for (auto scope: usings)
		out << "<using>" << scope->name << "</using>";
	out << "</usings>";
	out << "</scope>";
}

std::string Scope::ToString() const
{
	std::ostringstream out;
	if (parent != nullptr)
		out << parent->ToString();
	out << name << "::";
	return out.str();
}

