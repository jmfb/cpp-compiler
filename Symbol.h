#pragma once
#include <vector>
#include <string>
#include <ostream>
#include "Token.h"
#include "Scope.h"

class Symbol
{
public:
	Symbol() = default;
	Symbol(const std::string& type, const std::string& name, Scope* parent);
	Symbol(const Symbol& rhs) = default;
	~Symbol() = default;

	Symbol& operator=(const Symbol& rhs) = default;

	void WriteXml(std::ostream& out) const;
	
public:
	Scope scope;
	std::string type;
	std::string name;
	std::vector<Token> definitions;
};

