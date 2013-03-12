#include "Symbol.h"

Symbol::Symbol(const std::string& type, const std::string& name, Scope* parent)
	: scope(parent, name), type(type), name(name)
{
}

void Symbol::WriteXml(std::ostream& out) const
{
	out << "<symbol type=\"" << type << "\" name=\"" << name << "\">";
	scope.WriteXml(out);
	out << "</symbol>";
}

