#include "Macro.h"
#include <algorithm>
#include <stdexcept>

Macro::Macro(const std::vector<PreProcessorToken>& tokens)
{
	enum class State
	{
		Initial,
		Name,
		Parameter,
		AfterParameter,
		AfterVariadic,
		Replacements
	};

	auto state = State::Initial;
	for (auto& token: tokens)
	{
		switch(state)
		{
		case State::Initial:
			if (!token.CanIgnore())
			{
				if (token.type != PreProcessorTokenType::Identifier)
					throw std::logic_error("#define must be followed by an identifier.");
				name = token.value;
				position = token.position;
				state = State::Name;
			}
			break;

		case State::Name:
			if (token.value == "(")
			{
				isFunction = true;
				state = State::Parameter;
			}
			else
			{
				if (!token.CanIgnore())
					replacements.push_back(token);
				state = State::Replacements;
			}
			break;

		case State::Parameter:
			if (!token.CanIgnore())
			{
				if (token.type == PreProcessorTokenType::Identifier)
				{
					parameterNames.push_back(token.value);
					state = State::AfterParameter;
				}
				else if (token.value == "...")
				{
					isVariadic = true;
					state = State::AfterVariadic;
				}
				else if (token.value == ")")
				{
					state = State::Replacements;
				}
				else
					throw std::logic_error("Missing , or ) after identifier in macro function parameter list.\n" +
						tokens.front().position.ToString());
			}
			break;

		case State::AfterParameter:
			if (!token.CanIgnore())
			{
				if (token.value == ")")
					state = State::Replacements;
				else if (token.value == ",")
					state = State::Parameter;
				else
					throw std::logic_error("Invalid token in macro function parameter list.");
			}
			break;

		case State::AfterVariadic:
			if (!token.CanIgnore())
			{
				if (token.value != ")")
					throw std::logic_error("Missing ) after ... in macro function parameter list.");
				state = State::Replacements;
			}
			break;

		case State::Replacements:
			replacements.push_back(token);
			break;
		}
	}
}

bool Macro::IsIdentical(const Macro& rhs) const
{
	return name == rhs.name &&
		isFunction == rhs.isFunction &&
		parameterNames == rhs.parameterNames &&
		isVariadic == rhs.isVariadic &&
		replacements == rhs.replacements;
}

void Macro::ValidateParameterCount(std::size_t count, const Position& atPosition) const
{
	auto valid = isVariadic ?
		count >= parameterNames.size() :
		count == parameterNames.size();
	if (!valid)
		throw std::logic_error("Invalid number of parameters supplied to macro:" +
			name + ".\n" + position.ToString() + "\n" + atPosition.ToString());
}

bool Macro::IsParameterName(const std::string& value) const
{
	if (isVariadic && value == "__VA_ARGS__")
		return true;
	return std::find(parameterNames.begin(), parameterNames.end(), value) != parameterNames.end();
}

std::vector<PreProcessorToken> Macro::GetParameter(
	const std::string& value,
	const std::vector<std::vector<PreProcessorToken>>& parameters) const
{
	if (value == "__VA_ARGS__")
	{
		std::vector<PreProcessorToken> variadicParameters;
		for (auto index = parameterNames.size(); index < parameters.size(); ++index)
		{
			if (index > parameterNames.size())
				variadicParameters.push_back({ PreProcessorTokenType::Operator, { "", 0, 0 }, "," });
			variadicParameters.insert(variadicParameters.end(), parameters[index].begin(), parameters[index].end());
		}
		return variadicParameters;
	}
	auto iter = std::find(parameterNames.begin(), parameterNames.end(), value);
	return parameters[iter - parameterNames.begin()];
}

PreProcessorToken Macro::GetFirstTokenFromParameter(
	const std::string& value,
	const std::vector<std::vector<PreProcessorToken>>& parameters) const
{
	auto parameter = GetParameter(value, parameters);
	for (auto iter = parameter.begin(); iter != parameter.end(); ++iter)
		if (!iter->CanIgnore())
			return *iter;
	return { PreProcessorTokenType::EndOfFile, { "", 0, 0 }, "" };
}

PreProcessorToken Macro::GetLastTokenFromParameter(
	const std::string& value,
	const std::vector<std::vector<PreProcessorToken>>& parameters) const
{
	auto parameter = GetParameter(value, parameters);
	for (auto iter = parameter.rbegin(); iter != parameter.rend(); ++iter)
		if (!iter->CanIgnore())
			return *iter;
	return { PreProcessorTokenType::EndOfFile, { "", 0, 0 }, "" };
}

