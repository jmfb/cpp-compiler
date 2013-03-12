#include "PreProcessorCondition.h"
#include <stdexcept>
#include "PreProcessorExpression.h"

PreProcessorCondition::PreProcessorCondition(
	PreProcessorConditionType type,
	const Position& position,
	const std::vector<PreProcessorToken>& expression)
	: type(type), position(position), expression(expression)
{
}

bool PreProcessorCondition::Evaluate(const MacroTableExpander& table) const
{
	switch(type)
	{
	case PreProcessorConditionType::If:
	case PreProcessorConditionType::ElseIf:
		return EvaluateCondition(table);
	case PreProcessorConditionType::IfDefined:
		return EvaluateDefined(table);
	case PreProcessorConditionType::IfNotDefined:
		return !EvaluateDefined(table);
	case PreProcessorConditionType::Else:
		return true;
	}
	throw std::logic_error("Invalid condition type.");
}

bool PreProcessorCondition::EvaluateCondition(const MacroTableExpander& table) const
{
	PreProcessorExpression expression { table, this->expression };
	return expression.Evaluate();
}

bool PreProcessorCondition::EvaluateDefined(const MacroTableExpander& table) const
{
	auto iter = expression.begin();
	while (iter != expression.end() && iter->CanIgnore())
		++iter;
	if (iter == expression.end() || iter->type != PreProcessorTokenType::Identifier)
		throw std::logic_error("#ifdef/ndef must be followed by an identifier.");
	auto next = iter + 1;
	while (next != expression.end() && next->CanIgnore())
		++next;
	if (next != expression.end())
		throw std::logic_error("#ifdef/ndef contained additional tokens after identifier.");
	return table.GetMacroType(iter->value) != MacroType::Undefined;
}

