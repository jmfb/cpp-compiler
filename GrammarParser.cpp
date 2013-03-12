#include "GrammarParser.h"
#include <cctype>
#include <stdexcept>

GrammarParser::GrammarParser(std::map<std::string, NonTerminal>& items)
	: items(items)
{
}

void GrammarParser::Parse(const std::string& value)
{
	for (auto ch: value)
		ProcessCharacter(ch);
	ProcessEndOfFile();
}

void GrammarParser::ProcessCharacter(char ch)
{
	if (ch == '\n')
	{
		++line;
		column = 1;
	}
	else
		++column;

	switch(state)
	{
	case State::Initial:
		OnInitial(ch);
		break;
	case State::LeftSide:
		OnLeftSide(ch);
		break;
	case State::Middle:
		OnMiddle(ch);
		break;
	case State::RightSide:
		OnRightSide(ch);
		break;
	case State::NonTerminal:
		OnNonTerminal(ch);
		break;
	case State::Operator:
		OnOperator(ch);
		break;
	case State::Keyword:
		OnKeyword(ch);
		break;
	case State::Identifier:
		OnIdentifier(ch);
		break;
	case State::Constant:
		OnConstant(ch);
		break;
	case State::Comment:
		OnComment(ch);
		break;
	case State::FirstComment:
		OnFirstComment(ch);
		break;
	case State::BlockComment:
		OnBlockComment(ch);
		break;
	case State::BlockCommentEnd:
		OnBlockCommentEnd(ch);
		break;
	}
}

void GrammarParser::ProcessEndOfFile()
{
	if (state != State::Initial)
		throw std::logic_error("Invalid state after parsing.");
}

void GrammarParser::OnInitial(char ch)
{
	if (std::isspace(ch))
		return;
	if (ch == '#')
	{
		returnToState = state;
		state = State::FirstComment;
	}
	else if (ch == '<')
		state = State::LeftSide;
	else
		throw std::logic_error("Invalid character at beginning of production, expected <.");
}

void GrammarParser::OnLeftSide(char ch)
{
	if (ch == '>')
	{
		if (name.empty())
			throw std::logic_error("Cannot define empty non-terminal names.");
		nonTerminal = NonTerminal { name };
		name.clear();
		state = State::Middle;
	}
	else
		name += ch;
}

void GrammarParser::OnMiddle(char ch)
{
	if (std::isspace(ch))
		return;
	if (ch == '#')
	{
		returnToState = state;
		state = State::FirstComment;
	}
	else if (ch == '=')
	{
		state = State::RightSide;
		production = Production {};
	}
	else
		throw std::logic_error("Missing = after non terminal name.");
}

void GrammarParser::OnRightSide(char ch)
{
	if (std::isspace(ch))
		return;
	if (ch == '#')
	{
		returnToState = state;
		state = State::FirstComment;
	}
	else if (ch == ';')
	{
		if (!production.items.empty())
			nonTerminal.AddItem(production);
		items.insert({ nonTerminal.name, nonTerminal });
		production = Production {};
		nonTerminal = NonTerminal {};
		state = State::Initial;
	}
	else if (ch == '|')
	{
		if (production.items.empty())
			throw std::logic_error("Cannot define produciton with no items.");
		nonTerminal.AddItem(production);
		production = Production {};
	}
	else if (ch == '@')
	{
		if (!production.items.empty())
			throw std::logic_error("Cannot have a production containing the empty set and other items.");
		nonTerminal.AddItem(production);
	}
	else if (ch == '"')
		state = State::Keyword;
	else if (ch == '\'')
		state = State::Operator;
	else if (ch == '<')
		state = State::NonTerminal;
	else if (ch == 'i')
		state = State::Identifier;
	else if (ch == 'v')
		state = State::Constant;
	else
		throw std::logic_error("Invalid character in grammar parse string: " + std::string(1, ch));
}

void GrammarParser::OnNonTerminal(char ch)
{
	if (ch == '>')
	{
		AddNonTerminalToProduction();
		name.clear();
		state = State::RightSide;
	}
	else
		name += ch;
}

void GrammarParser::OnOperator(char ch)
{
	if (ch == '\'')
	{
		if (name.empty())
			throw std::logic_error("Cannot use empty operators.");
		production.AddItem({ TokenType::Operator, name, {} });
		name.clear();
		state = State::RightSide;
	}
	else
	{
		name += ch;
		if (name.size() > 5)
			throw std::logic_error("Operators longer than 5 characters are invalid.");
	}
}

void GrammarParser::OnKeyword(char ch)
{
	if (ch == '"')
	{
		if (name.empty())
			throw std::logic_error("Cannot use empty keywords.");
		production.AddItem({ TokenType::Keyword, name, {} });
		name.clear();
		state = State::RightSide;
	}
	else
		name += ch;
}

void GrammarParser::OnIdentifier(char ch)
{
	if (ch == 'd')
	{
		production.AddItem({ TokenType::Identifier, "", {} });
		state = State::RightSide;
	}
}

void GrammarParser::OnConstant(char ch)
{
	if (ch == 'l')
	{
		production.AddItem({ TokenType::Constant, "", {} });
		state = State::RightSide;
	}
}

void GrammarParser::OnComment(char ch)
{
	if (ch == '\n')
		state = returnToState;
}

void GrammarParser::OnFirstComment(char ch)
{
	if (ch == '#')
		state = State::BlockComment;
	else if (ch == '\n')
		state = returnToState;
	else
		state = State::Comment;
}

void GrammarParser::OnBlockComment(char ch)
{
	if (ch == '#')
		state = State::BlockCommentEnd;
}

void GrammarParser::OnBlockCommentEnd(char ch)
{
	if (ch == '#')
		state = returnToState;
	else
		state = State::BlockComment;
}

void GrammarParser::AddNonTerminalToProduction()
{
	if (name.empty())
		throw std::logic_error("Cannot use empty non-terminal names.");

	//Parse out optional parameter name assignment
	auto index = name.rfind(",$");
	auto other = name.find(")", index + 2);
	auto hasParamName = index != std::string::npos && other == std::string::npos;
	auto paramName = hasParamName ? name.substr(index + 2) : "";
	name = hasParamName ? name.substr(0, index) : name;

	//Parse out optional assignment
	auto isOptional = name.rfind("-opt") == (name.size() - 4) && name.size() > 4;
	name = isOptional ? name.substr(0, name.size() - 4) : name;

	//Parse out optional grammar action commands and arguments
	index = name.find("-@");
	auto hasAction = index != std::string::npos;
	GrammarAction action;
	if (hasAction)
	{
		auto actionstr = name.substr(index + 2);
		name = name.substr(0, index);

		index = actionstr.find("(");
		auto actioncmd = index == std::string::npos ? actionstr : actionstr.substr(0, index);
		auto type = GrammarActionType::None;
		std::string arg1;
		std::string arg2;
		if (index == std::string::npos)
		{
			if (actioncmd != "pop")
				throw std::logic_error("Only the pop operation has no parameters.");
			type = GrammarActionType::Pop;
		}
		else
		{
			if (actionstr[actionstr.size() - 1] != ')')
				throw std::logic_error("Expected ')' at end of grammar action command.");
			arg1 = actionstr.substr(index + 1, actionstr.size() - index - 2);
			index = arg1.find(",");
			if (index == std::string::npos)
			{
				if (actioncmd == "push")
				{
					if (arg1 == "unnamed")
					{
						type = GrammarActionType::PushUnnamed;
						arg1.clear();
					}
					else if (arg1 == "null")
					{
						type = GrammarActionType::PushNull;
						arg1.clear();
					}
					else if (arg1 == "block")
					{
						type = GrammarActionType::PushBlock;
						arg1.clear();
					}
					else if (arg1.size() < 2 || arg1[0] != '$')
					{
						throw std::logic_error("Push argument must be unnamed, block, or $id");
					}
					else
					{
						type = GrammarActionType::Push;
						arg1 = arg1.substr(1);
					}
				}
				else if (actioncmd == "scope")
				{
					if (arg1 == "global")
					{
						type = GrammarActionType::ScopeGlobal;
						arg1.clear();
					}
					else if (arg1.size() < 2 || arg1[0] != '$')
					{
						throw std::logic_error("Scope argument must be global or $id");
					}
					else
					{
						type = GrammarActionType::Scope;
						arg1 = arg1.substr(1);
					}
				}
				else if (actioncmd == "usens")
				{
					if (arg1.size() < 2 || arg1[0] != '$')
						throw std::logic_error("UseNS argument must be $id");
					type = GrammarActionType::UsingNamespace;
					arg1 = arg1.substr(1);
				}
				else if (actioncmd == "usesym")
				{
					if (arg1.size() < 2 || arg1[0] != '$')
						throw std::logic_error("UseSym argument must be $id");
					type = GrammarActionType::UsingSymbol;
					arg1 = arg1.substr(1);
				}
				else
				{
					throw std::logic_error("Only push, scope, usens, and usesym grammar actions have 1 argument.");
				}
			}
			else
			{
				arg2 = arg1.substr(index + 1);
				arg1 = arg1.substr(0, index);

				if (actioncmd == "alias")
				{
					if (arg1.size() < 2 || arg2.size() < 2 || arg1[0] != '$' || arg2[0] != '$')
					{
						throw std::logic_error("Both arguments to alias must be $id.");
					}
					else
					{
						type = GrammarActionType::Alias;
						arg1 = arg1.substr(1);
						arg2 = arg2.substr(1);
					}
				}
				else if (actioncmd == "define")
				{
					if (arg1.empty() || arg2.size() < 2 || arg2[0] != '$')
					{
						throw std::logic_error("First argument to define must be non-blank and second argument must be $id.");
					}
					else
					{
						type = GrammarActionType::Define;
						arg2 = arg2.substr(1);
					}
				}
				else if (actioncmd == "defpush")
				{
					if (arg1.empty() || arg2.size() < 2 || arg2[0] != '$')
					{
						throw std::logic_error("First argument to defpush must be non-blank and second argument must be $id.");
					}
					else
					{
						type = GrammarActionType::DefinePush;
						arg2 = arg2.substr(1);
					}
				}
				else
				{
					throw std::logic_error("Only alias, define, and defpush grammar actions have 2 arguments.");
				}
			}
		}

		action = GrammarAction { type, arg1, arg2 };
	}

	//Check name again after parsing out optional suffixes
	if (name.empty())
		throw std::logic_error("Cannot use empty non-terminal names (after suffix removal).");

	//Check for special identifier command prefixes
	if (name.find("id=") == 0)
	{
		if (isOptional)
			throw std::logic_error("Cannot apply optional parameter to id= definition.");
		production.AddItem({ TerminalIdentifierType::Define, name.substr(3), paramName, action });
	}
	else if (name.find("id:") == 0)
	{
		if (isOptional)
			throw std::logic_error("Cannot apply optional parameter to id: definition.");
		production.AddItem({ TerminalIdentifierType::Lookup, name.substr(3), paramName, action });
	}
	else if (name == "id*")
	{
		if (isOptional)
			throw std::logic_error("Cannot apply optional parameter to id* definition.");
		production.AddItem({ TerminalIdentifierType::Any, "", paramName, action });
	}
	else if (name.find("id.") == 0)
	{
		if (isOptional)
			throw std::logic_error("Cannot apply optional parameter to id. definition.");
		production.AddItem({ TerminalIdentifierType::Match, name.substr(3), "", action });
	}
	else
	{
		//Cannot combine grammar actions with the optional parameter
		if (isOptional && action.type != GrammarActionType::None)
			throw std::logic_error("Cannot combine optional grammar symbols with grammar actions.");

		if (isOptional)
		{
			//Add the name+opt non-terminal to the production and make sure the proper optional item is defined
			auto optname = name + "-opt";
			production.AddItem(optname);
			if (items.find(optname) == items.end())
			{
				NonTerminal optional { optname };
				if (name.find("op-") == 0)
					optional.AddItem(Production {}.AddItem({ TokenType::Operator, name.substr(3), {} }));
				else if (name.find("key-") == 0)
					optional.AddItem(Production {}.AddItem({ TokenType::Keyword, name.substr(4), {} }));
				else if (name == "id")
					optional.AddItem(Production {}.AddItem({ TokenType::Identifier, "", {} }));
				else
					optional.AddItem(Production {}.AddItem(name));
				optional.AddItem({});
				items.insert({ optname, optional });
			}
		}
		else
		{
			//Handle special types that were created for the purpose of adding grammar actions
			if (name.find("op-") == 0)
				production.AddItem({ TokenType::Operator, name.substr(3), action });
			else if (name.find("key-") == 0)
				production.AddItem({ TokenType::Keyword, name.substr(4), action });
			else if (name == "id")
				production.AddItem({ TerminalIdentifierType::Any, "", name, action });
			else
				production.AddItem(name);
		}
	}
}

