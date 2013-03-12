#include "PreProcessorToken.h"
#include <sstream>

PreProcessorToken::PreProcessorToken(
	PreProcessorTokenType type,
	const Position& position,
	const std::string& value)
	: type(type), position(position), value(value)
{
}

bool PreProcessorToken::IsDirectiveOperator() const
{
	return IsOperator("#");
}

bool PreProcessorToken::IsOperator(const std::string& op) const
{
	return type == PreProcessorTokenType::Operator && value == op;
}

bool PreProcessorToken::CanIgnore() const
{
	return type == PreProcessorTokenType::Whitespace ||
		type == PreProcessorTokenType::Comment;
}

bool PreProcessorToken::operator==(const PreProcessorToken& rhs) const
{
	return type == rhs.type && value == rhs.value;
}

std::string PreProcessorToken::Stringize() const
{
	if (type == PreProcessorTokenType::Comment)
		return " ";
	if (type == PreProcessorTokenType::String ||
		type == PreProcessorTokenType::Character)
	{
		std::ostringstream out;
		for (auto ch: value)
		{
			if (ch == '\\' || ch == '"')
				out << '\\';
			out << ch;
		}
		return out.str();
	}
	return value;
}

PreProcessorToken PreProcessorToken::Concatenate(const PreProcessorToken& rhs) const
{
	auto newValue = value + rhs.value;
	auto newType = value.empty() ? rhs.type : type;
	if (newValue.empty())
		newType = PreProcessorTokenType::EndOfFile;
	else if (newValue == "##")
		newType = PreProcessorTokenType::OtherChar;
	else if (rhs.type == PreProcessorTokenType::String ||
		rhs.type == PreProcessorTokenType::Character)
		newType = rhs.type;
	return { newType, position, newValue };
}

std::string PreProcessorToken::ToString() const
{
	std::ostringstream out;
	out << "{" << ::ToString(type) << ": " << value << "}";
	return out.str();
}

