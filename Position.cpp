#include "Position.h"
#include <sstream>

Position::Position(const std::string& source, std::size_t line, std::size_t column)
	: source(source), line(line), column(column)
{
}

std::string Position::ToString() const
{
	std::ostringstream out;
	out << source << "(Line " << line << ", Col " << column << ")";
	return out.str();
}

