#include "MacroExpander.h"
#include <stdexcept>
#include <algorithm>
#include <sstream>

MacroExpander::MacroExpander(
	const Macro& macro,
	const Position& position,
	const std::vector<std::vector<PreProcessorToken>>& parameters,
	const std::set<std::string>& visited)
	: macro(macro), position(position), parameters(parameters), visited(visited)
{
}

void MacroExpander::Expand(const MacroTableExpander& table)
{
	if (!macro.isFunction)
	{
		for (auto& token: macro.replacements)
			expansion.push_back(token);
	}
	else
	{
		macro.ValidateParameterCount(parameters.size(), position);
		for (auto& parameter: parameters)
			expandedParameters.push_back(EvaluateSequence(table, parameter, visited));
		expansion = macro.replacements;
		StringizeParameters();
		ConcatenateAndExpandParameters();
	}

	visited.insert(macro.name);
	expansion = EvaluateSequence(table, expansion, visited);
}

void MacroExpander::StringizeParameters()
{
	for (auto iter = expansion.begin(); iter != expansion.end(); )
	{
		if (!iter->IsOperator("#"))
		{
			++iter;
			continue;
		}

		auto name = iter + 1;
		while (name != expansion.end() && name->CanIgnore())
			++name;
		if (name == expansion.end() ||
			name->type != PreProcessorTokenType::Identifier ||
			!macro.IsParameterName(name->value))
			throw std::logic_error("# operator must be applied to a macro parameter.");

		auto parameter = macro.GetParameter(name->value, parameters);
		auto stringized = Stringize(parameter);
		iter = expansion.erase(iter, name + 1);
		iter = expansion.insert(iter, stringized);
		++iter;
	}
}

PreProcessorToken MacroExpander::Stringize(const std::vector<PreProcessorToken>& tokens)
{
	std::ostringstream out;
	out << "\"";
	for (auto& token: tokens)
		out << token.Stringize();
	out << "\"";
	return { PreProcessorTokenType::String, { "", 0, 0 }, out.str() };
}

void MacroExpander::ConcatenateAndExpandParameters()
{
	for (auto iter = expansion.begin(); iter != expansion.end(); )
	{
		if (iter->IsOperator("##"))
			throw std::logic_error("## must be preceded by a valid token.");

		if (iter->CanIgnore())
		{
			++iter;
			continue;
		}

		auto lhsIsParameter =
			iter->type == PreProcessorTokenType::Identifier &&
			macro.IsParameterName(iter->value);

		auto next = iter + 1;
		while (next != expansion.end() && next->CanIgnore())
			++next;
		if (next != expansion.end() && next->IsOperator("##"))
		{
			++next;
			while (next != expansion.end() && next->CanIgnore())
				++next;
			if (next == expansion.end())
				throw std::logic_error("## must be followed by a valid identifier.");

			auto rhsIsParameter =
				next->type == PreProcessorTokenType::Identifier &&
				macro.IsParameterName(next->value);

			auto lhs = lhsIsParameter ? macro.GetLastTokenFromParameter(iter->value, parameters) : *iter;
			auto rhs = rhsIsParameter ? macro.GetFirstTokenFromParameter(next->value, parameters) : *next;
			auto concatenatedToken = lhs.Concatenate(rhs);
			iter = expansion.erase(iter, next + 1);
			if (concatenatedToken.type != PreProcessorTokenType::EndOfFile)
			{
				iter = expansion.insert(iter, concatenatedToken);
				++iter;
			}
		}
		else if (lhsIsParameter)
		{
			auto parameter = macro.GetParameter(iter->value, expandedParameters);
			iter = expansion.erase(iter);
			auto position = iter - expansion.begin();
			expansion.insert(iter, parameter.begin(), parameter.end());
			iter = expansion.begin() + position + parameter.size();
		}
		else
		{
			++iter;
		}
	}
}

std::vector<PreProcessorToken> MacroExpander::EvaluateSequence(
	const MacroTableExpander& table,
	const std::vector<PreProcessorToken>& tokens,
	std::set<std::string> visited)
{
	std::vector<PreProcessorToken> expansion = tokens;
	for (auto iter = expansion.begin(); iter != expansion.end(); )
	{
		if (iter->type != PreProcessorTokenType::Identifier ||
			visited.find(iter->value) != visited.end())
		{
			++iter;
			continue;
		}

		auto macroType = table.GetMacroType(iter->value);
		switch(macroType)
		{
		case MacroType::Undefined:
			++iter;
			break;
		case MacroType::Object:
			iter = EvaluateObject(table, expansion, iter, visited);
			break;
		case MacroType::Function:
			iter = EvaluateFunction(table, expansion, iter, visited);
			break;
		}
	}
	return expansion;
}

std::vector<PreProcessorToken>::iterator MacroExpander::EvaluateObject(
	const MacroTableExpander& table,
	std::vector<PreProcessorToken>& expansion,
	std::vector<PreProcessorToken>::iterator iter,
	const std::set<std::string> visited)
{
	auto objectExpansion = table.Expand(*iter, {}, visited);
	iter = expansion.erase(iter);
	auto position = iter - expansion.begin();
	expansion.insert(iter, objectExpansion.begin(), objectExpansion.end());
	return expansion.begin() + position + objectExpansion.size();
}

std::vector<PreProcessorToken>::iterator MacroExpander::EvaluateFunction(
	const MacroTableExpander& table,
	std::vector<PreProcessorToken>& expansion,
	std::vector<PreProcessorToken>::iterator iter,
	const std::set<std::string> visited)
{
	enum class State
	{
		Initial,
		Parameter,
		Comma
	};

	auto getNextImportantToken = [&](std::vector<PreProcessorToken>::iterator after)
	{
		if (after == expansion.end())
			return expansion.end();
		return std::find_if(after + 1, expansion.end(), [](const PreProcessorToken& token)
		{
			return !token.CanIgnore();
		});
	};

	auto lparen = getNextImportantToken(iter);
	if (lparen == expansion.end() || lparen->value != "(")
		throw std::logic_error("Function-like macro expansion requires argument list.");

	std::vector<std::vector<PreProcessorToken>> parameters;
	std::vector<PreProcessorToken> nextParameter;
	auto unclosedParens = 0ul;

	auto state = State::Initial;
	auto token = lparen;
	for (auto done = false; !done; )
	{
		++token;
		if (token == expansion.end())
			throw std::logic_error("End of file encountered during function-like macro expansion.");
		switch(state)
		{
		case State::Initial:
			if (!token->CanIgnore())
			{
				if (token->value == ")")
				{
					done = true;
				}
				else if (token->value == ",")
				{
					state = State::Comma;
					parameters.push_back({});
				}
				else
				{
					if (token->value == "(")
						++unclosedParens;
					nextParameter.push_back(*token);
					state = State::Parameter;
				}
			}
			break;

		case State::Parameter:
			if (token->value == "(")
			{
				++unclosedParens;
				nextParameter.push_back(*token);
			}
			else if (token->value == ")")
			{
				if (unclosedParens == 0)
				{
					parameters.push_back(nextParameter);
					done = true;
				}
				else
				{
					nextParameter.push_back(*token);
					--unclosedParens;
				}
			}
			else if (token->value == "," && unclosedParens == 0)
			{
				parameters.push_back(nextParameter);
				nextParameter.clear();
				state = State::Comma;
			}
			else
			{
				nextParameter.push_back(*token);
			}
			break;

		case State::Comma:
			if (!token->CanIgnore())
			{
				if (token->value == ")")
				{
					parameters.push_back({});
					done = true;
				}
				else if (token->value == ",")
				{
					parameters.push_back({});
				}
				else
				{
					if (token->value == "(")
						++unclosedParens;
					nextParameter.push_back(*token);
					state = State::Parameter;
				}
			}
			break;
		}
	}

	auto functionExpansion = table.Expand(*iter, parameters, visited);
	iter = expansion.erase(iter, token + 1);
	auto position = iter - expansion.begin();
	expansion.insert(iter, functionExpansion.begin(), functionExpansion.end());
	return expansion.begin() + position + functionExpansion.size();
}

