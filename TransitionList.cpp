#include "TransitionList.h"
#include <sstream>
#include <stdexcept>

TransitionList::TransitionList(std::size_t index)
	: index(index)
{
}

void TransitionList::AddAction(
	const Terminal& terminal,
	const TransitionAction& action,
	ConflictResolver& resolver)
{
	auto iter = actions.find(terminal);
	if (iter == actions.end())
	{
		actions.insert({ terminal, action });
		return;
	}

	if (iter->second != action)
	{
		switch(resolver.Resolve(terminal, iter->second, action))
		{
		case ConflictResolution::KeepNew:
			iter->second = action;
			break;

		case ConflictResolution::KeepOld:
			//nothing: keep old action and throw away new action
			break;

		case ConflictResolution::None:
			{
				std::ostringstream out;
				out << "Conflicting actions generated for I" << index << ", terminal: " << terminal.ToString() << std::endl
					<< "First action: " << iter->second.ToString() << std::endl
					<< "New action: " << action.ToString() << std::endl;
				throw std::logic_error(out.str());
			}
		}
	}
}

void TransitionList::AddGoto(
	const std::string& nonTerminal,
	std::size_t state)
{
	auto iter = gotos.find(nonTerminal);
	if (iter == gotos.end())
		gotos.insert({ nonTerminal, state });
	else if (iter->second != state)
		throw std::logic_error("Conflicting gotos generated.");
}

TransitionAction TransitionList::GetAction(
	const Token& token,
	SymbolTable& symtab)
{
	std::vector<std::pair<Terminal, TransitionAction>> possibleActions;
	for (auto& pair: actions)
		if (pair.first.Match(token, symtab, false))
			possibleActions.push_back(pair);
	if (possibleActions.empty())
		throw std::logic_error("Error: missing action for terminal: \n" + token.ToString());
	if (possibleActions.size() == 1)
	{
		possibleActions[0].first.Match(token, symtab, possibleActions[0].second.type == TransitionType::Shift);
		return possibleActions[0].second;
	}
	return ResolveMultipleTransition(token, symtab, possibleActions);
}

std::size_t TransitionList::GetGoto(const std::string& nonTerminal)
{
	auto iter = gotos.find(nonTerminal);
	if (iter == gotos.end())
		throw std::logic_error("Error: missing goto for non-terminal.");
	return iter->second;
}

void TransitionList::Write(BinaryWriter& writer) const
{
	writer.Write(index);
	writer.Write(static_cast<long>(actions.size()));
	for (auto& item: actions)
	{
		item.first.Write(writer);
		item.second.Write(writer);
	}
	writer.Write(static_cast<long>(gotos.size()));
	for (auto& item: gotos)
	{
		writer.Write(item.first);
		writer.Write(item.second);
	}
}

void TransitionList::Read(BinaryReader& reader)
{
	index = reader.ReadSize();
	auto actionCount = reader.ReadInt32();
	for (auto index = 0; index < actionCount; ++index)
	{
		Terminal terminal;
		terminal.Read(reader);
		TransitionAction action;
		action.Read(reader);
		actions.insert({ terminal, action });
	}
	auto gotoCount = reader.ReadInt32();
	for (auto index = 0; index < gotoCount; ++index)
	{
		auto nonTerminal = reader.ReadString();
		auto state = reader.ReadSize();
		gotos.insert({ nonTerminal, state });
	}
}

TransitionAction TransitionList::ResolveMultipleTransition(
	const Token& token,
	SymbolTable& symtab,
	const std::vector<std::pair<Terminal, TransitionAction>>& actions)
{
	//Multiple matching operators with different actions
	if (actions[0].first.type == TokenType::Operator)
	{
		//We can resolve: Scope(Global) vs. Scope(ID) to use Scope(ID) always
		auto foundActionGlobal = false;
		auto foundActionId = false;
		TransitionAction actionGlobal;
		TransitionAction actionId;
		Terminal terminalActionId;
		for (auto& pair: actions)
		{
			if (pair.first.action.type == GrammarActionType::ScopeGlobal)
			{
				if (foundActionGlobal)
					throw std::logic_error(MultipleTransitionException(token, actions));
				foundActionGlobal = true;
				actionGlobal = pair.second;
			}
			else if (pair.first.action.type == GrammarActionType::Scope)
			{
				if (foundActionId)
					throw std::logic_error(MultipleTransitionException(token, actions));
				foundActionId = true;
				terminalActionId = pair.first;
				actionId = pair.second;
			}
			else
				throw std::logic_error(MultipleTransitionException(token, actions));
		}

		//Invoke Terminal actions and return the ID
		terminalActionId.Match(token, symtab, actionId.type == TransitionType::Shift);
		return actionId;
	}
	//Multiple matching identifiers, could be "any" vs. "lookup" problems, or could be named variable based
	else if (actions[0].first.type == TokenType::Identifier)
	{
		//We can resolve "typed" vs. "any" by choosing "typed" in all cases
		auto foundActionTyped = false;
		TransitionAction actionTyped;
		Terminal terminalTyped;
		for (auto& pair: actions)
		{
			if (pair.first.idType == TerminalIdentifierType::Any)
				continue;
			else if (pair.first.idType == TerminalIdentifierType::Lookup || pair.first.idType == TerminalIdentifierType::Match)
			{
				if (foundActionTyped)
					throw std::logic_error(MultipleTransitionException(token, actions));
				actionTyped = pair.second;
				terminalTyped = pair.first;
			}
			else
				throw std::logic_error(MultipleTransitionException(token, actions));
		}
		if (!foundActionTyped)
			throw std::logic_error(MultipleTransitionException(token, actions));

		terminalTyped.Match(token, symtab, actionTyped.type == TransitionType::Shift);
		return actionTyped;
	}
	throw std::logic_error(MultipleTransitionException(token, actions));
}

std::string TransitionList::MultipleTransitionException(
	const Token& token,
	const std::vector<std::pair<Terminal, TransitionAction>>& actions)
{
	std::ostringstream out;
	out << "Multiple actions found for token: " << token.value << std::endl;
	for (auto& action: actions)
		out << action.first.ToString() << std::endl;
	return out.str();
}

std::string TransitionList::ToString() const
{
	std::ostringstream out;
	out << "TransitionList: " << index << ", Actions" << std::endl;
	for (auto& item: actions)
		out << item.first.ToString() << ": " << item.second.ToString() << std::endl;
	out << "> Goto Table" << std::endl;
	for (auto& item: gotos)
		out << item.first << " -> " << item.second << std::endl;
	return out.str();
}

