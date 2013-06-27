#include "PreProcessorTokenStreamDiamond.h"
#include <stdexcept>

enum class Action
{
	Keep,
	PutBack
};

class PreProcessorTokenStreamDiamond::Transition
{
public:
	Transition() = default;
	Transition(Action action, PreProcessorTokenType type)
		: action(action), done(true), type(type)
	{
	}
	Transition(Action action, State state)
		: action(action), changeState(true), state(state)
	{
	}
	Transition(const Transition& rhs) = default;
	~Transition() = default;

	Transition& operator=(const Transition& rhs) = default;

public:
	Action action = Action::Keep;
	bool done = false;
	PreProcessorTokenType type = PreProcessorTokenType::EndOfFile;
	bool changeState = false;
	State state = State::Initial;
};

PreProcessorTokenStreamDiamond::PreProcessorTokenStreamDiamond(CharStream& stream)
	: PreProcessorTokenStream(stream)
{
}

PreProcessorToken PreProcessorTokenStreamDiamond::GetNextToken()
{
	state = State::Initial;
	position = { "", 0, 0 };
	value.clear();

	while (!stream.IsEOF())
	{
		ch = stream.GetNext();
		if (ch.value == 0)
			break;

		auto transition = StateDispatch();
		switch(transition.action)
		{
		case Action::Keep:
			value += ch.value;
			break;
		case Action::PutBack:
			stream.PutBack(ch);
			break;
		}

		if (transition.changeState)
			state = transition.state;
		else if (transition.done)
			return { transition.type, position, value };
	}

	return GetTokenBeforeEndOfFile();
}

PreProcessorTokenStreamDiamond::Transition PreProcessorTokenStreamDiamond::StateDispatch()
{
	switch(state)
	{
	case State::Initial:
		return OnInitial();
	case State::Whitespace:
		return OnWhitespace();
	case State::ForwardSlash:
		return OnForwardSlash();
	case State::SingleLineComment:
		return OnSingleLineComment();
	case State::MultiLineComment:
		return OnMultiLineComment();
	case State::MultiLineCommentStar:
		return OnMultiLineCommentStar();
	case State::Identifier:
		return OnIdentifier();
	case State::String:
		return OnString();
	case State::StringEscape:
		return OnStringEscape();
	case State::Character:
		return OnCharacter();
	case State::CharacterEscape:
		return OnCharacterEscape();
	case State::Number:
		return OnNumber();
	case State::OperatorColon:
		return OnOperatorColon();
	case State::OperatorQuestion:
		return OnOperatorQuestion();
	case State::OperatorWithEquals:
		return OnOperatorWithEquals();
	case State::OperatorSelfOrEquals:
		return OnOperatorSelfOrEquals();
	case State::OperatorAngleBracket:
		return OnOperatorAngleBracket();
	case State::Minus:
		return OnMinus();
	case State::Dot:
		return OnDot();
	}
	throw std::logic_error("Invalid state.");
}

PreProcessorToken PreProcessorTokenStreamDiamond::GetTokenBeforeEndOfFile() const
{
	switch(state)
	{
	case State::Initial:
		return { PreProcessorTokenType::EndOfFile, position, value };
	case State::Whitespace:
		return { PreProcessorTokenType::Whitespace, position, value };
	case State::ForwardSlash:
		return { PreProcessorTokenType::Operator, position, value };
	case State::SingleLineComment:
		return { PreProcessorTokenType::Comment, position, value };
	case State::MultiLineComment:
	case State::MultiLineCommentStar:
		throw std::logic_error("Multi-line comment missing */ before end of file.");
	case State::Identifier:
		return { PreProcessorTokenType::Identifier, position, value };
	case State::String:
	case State::StringEscape:
		throw std::logic_error("String not closed before end of file.");
	case State::Character:
	case State::CharacterEscape:
		throw std::logic_error("Character not closed before end of file.");
	case State::Number:
		return { PreProcessorTokenType::Number, position, value };
	case State::OperatorColon:
	case State::OperatorQuestion:
	case State::OperatorWithEquals:
	case State::OperatorSelfOrEquals:
	case State::OperatorAngleBracket:
	case State::Minus:
	case State::Dot:
		return { PreProcessorTokenType::Operator, position, value };
	}
	throw std::logic_error("Invalid state encountered at end of file.");
}

PreProcessorTokenStreamDiamond::Transition PreProcessorTokenStreamDiamond::OnInitial()
{
	//TODO:	Strings and character literals may begin with optional prefix (L, U, u, u8)
	//		All literals may be followed with user defined literal suffix (_blah)
	//		Numeric literals may contain floating point components or begin with a period.
	//		String literals may use "Raw" format: R"value-opt(raw-string)value-opt".
	position = ch.position;
	if (ch.value == '\n')
		return { Action::Keep, PreProcessorTokenType::Newline };
	else if (std::isspace(ch.value))
		return { Action::Keep, State::Whitespace };
	else if (ch.value == '/')
		return { Action::Keep, State::ForwardSlash };
	else if (ch.value == '_' || std::isalpha(ch.value))
		return { Action::Keep, State::Identifier };
	else if (ch.value == '"')
		return { Action::Keep, State::String };
	else if (ch.value == '\'')
		return { Action::Keep, State::Character };
	else if (std::isdigit(ch.value))
		return { Action::Keep, State::Number };
	else if (std::ispunct(ch.value))
		return OnOperator();
	else
		return { Action::Keep, PreProcessorTokenType::OtherChar };
}

PreProcessorTokenStreamDiamond::Transition PreProcessorTokenStreamDiamond::OnWhitespace() const
{
	if (ch.value == '\n' || !std::isspace(ch.value))
		return { Action::PutBack, PreProcessorTokenType::Whitespace };
	return {};
}

PreProcessorTokenStreamDiamond::Transition PreProcessorTokenStreamDiamond::OnForwardSlash() const
{
	if (ch.value == '/')
		return { Action::Keep, State::SingleLineComment };
	else if (ch.value == '*')
		return { Action::Keep, State::MultiLineComment };
	else if (ch.value == '=')
		return { Action::Keep, PreProcessorTokenType::Operator };
	return { Action::PutBack, PreProcessorTokenType::Operator };
}

PreProcessorTokenStreamDiamond::Transition PreProcessorTokenStreamDiamond::OnSingleLineComment() const
{
	if (ch.value == '\n')
		return { Action::PutBack, PreProcessorTokenType::Comment };
	return {};
}

PreProcessorTokenStreamDiamond::Transition PreProcessorTokenStreamDiamond::OnMultiLineComment() const
{
	if (ch.value == '*')
		return { Action::Keep, State::MultiLineCommentStar };
	return {};
}

PreProcessorTokenStreamDiamond::Transition PreProcessorTokenStreamDiamond::OnMultiLineCommentStar() const
{
	if (ch.value == '/')
		return { Action::Keep, PreProcessorTokenType::Comment };
	else if (ch.value != '*')
		return { Action::Keep, State::MultiLineComment };
	return {};
}

PreProcessorTokenStreamDiamond::Transition PreProcessorTokenStreamDiamond::OnIdentifier() const
{
	if (ch.value != '_' && !std::isalnum(ch.value))
		return { Action::PutBack, PreProcessorTokenType::Identifier };
	return {};
}

PreProcessorTokenStreamDiamond::Transition PreProcessorTokenStreamDiamond::OnString() const
{
	if (ch.value == '\\')
		return { Action::Keep, State::StringEscape };
	else if (ch.value == '"')
		return { Action::Keep, PreProcessorTokenType::String };
	return {};
}

PreProcessorTokenStreamDiamond::Transition PreProcessorTokenStreamDiamond::OnStringEscape() const
{
	return { Action::Keep, State::String };
}

PreProcessorTokenStreamDiamond::Transition PreProcessorTokenStreamDiamond::OnCharacter() const
{
	if (ch.value == '\\')
		return { Action::Keep, State::CharacterEscape };
	else if (ch.value == '\'')
		return { Action::Keep, PreProcessorTokenType::Character };
	return {};
}

PreProcessorTokenStreamDiamond::Transition PreProcessorTokenStreamDiamond::OnCharacterEscape() const
{
	return { Action::Keep, State::Character };
}

PreProcessorTokenStreamDiamond::Transition PreProcessorTokenStreamDiamond::OnNumber() const
{
	if (!std::isalnum(ch.value))
		return { Action::PutBack, PreProcessorTokenType::Number };
	return {};
}

PreProcessorTokenStreamDiamond::Transition PreProcessorTokenStreamDiamond::OnOperator() const
{
	//Forward slash handled specially because of comments (/, /=, /*, //)
	switch(ch.value)
	{
	case '(':
	case ')':
	case '[':
	case ']':
	case '{':
	case '}':
	case ',':
	case ';':
	case '~':
	case '@':
		return { Action::Keep, PreProcessorTokenType::Operator };

	case '?': // ?, ??, ?=, ?., ?.*, ?->, ?->*
		return { Action::Keep, State::OperatorQuestion };

	case ':': // :, ::, :>
		return { Action::Keep, State::OperatorColon };

	case '=': // =, ==
	case '!': // !, !=
	case '*': // *, *=
	case '%': // %, %=
	case '^': // ^, ^=
		return { Action::Keep, State::OperatorWithEquals };

	case '+': // +, ++, +=
	case '|': // |, ||, |=
	case '&': // &, &&, &=
		return { Action::Keep, State::OperatorSelfOrEquals };

	case '<': // <, <=, <<, <<=, <:
	case '>': // >, >=, >>, >>=
		return { Action::Keep, State::OperatorAngleBracket };

	case '-': // -, --, -=, ->, ->*
		return { Action::Keep, State::Minus };

	case '.': // ., ..., .*
		return { Action::Keep, State::Dot };
	}
	return { Action::Keep, PreProcessorTokenType::OtherChar };
}

PreProcessorTokenStreamDiamond::Transition PreProcessorTokenStreamDiamond::OnOperatorColon() const
{
	if (ch.value == '>' || ch.value == ':')
		return { Action::Keep, PreProcessorTokenType::Operator };
	return { Action::PutBack, PreProcessorTokenType::Operator };
}

PreProcessorTokenStreamDiamond::Transition PreProcessorTokenStreamDiamond::OnOperatorQuestion()
{
	if (ch.value == '?' || ch.value == '=')
		return { Action::Keep, PreProcessorTokenType::Operator };
	else if (ch.value == '.')
	{
		auto peek = stream.GetNext();
		if (peek.value == '*')
		{
			value += ch.value;
			ch = peek;
		}
		else if (peek.value != 0)
		{
			stream.PutBack(peek);
		}
		return { Action::Keep, PreProcessorTokenType::Operator };
	}
	else if (ch.value == '-')
	{
		auto peek = stream.GetNext();
		if (peek.value == '>')
		{
			value += ch.value;
			ch = peek;
			auto nextPeek = stream.GetNext();
			if (nextPeek.value == '*')
			{
				value += ch.value;
				ch = nextPeek;
			}
			else if (nextPeek.value != 0)
			{
				stream.PutBack(nextPeek);
			}
			return { Action::Keep, PreProcessorTokenType::Operator };
		}
		else if (peek.value != 0)
		{
			stream.PutBack(peek);
		}
	}
	return { Action::PutBack, PreProcessorTokenType::Operator };
}

PreProcessorTokenStreamDiamond::Transition PreProcessorTokenStreamDiamond::OnOperatorWithEquals() const
{
	if (ch.value == '=')
		return { Action::Keep, PreProcessorTokenType::Operator };
	return { Action::PutBack, PreProcessorTokenType::Operator };
}

PreProcessorTokenStreamDiamond::Transition PreProcessorTokenStreamDiamond::OnOperatorSelfOrEquals() const
{
	if (ch.value == value[0] || ch.value == '=')
		return { Action::Keep, PreProcessorTokenType::Operator };
	return { Action::PutBack, PreProcessorTokenType::Operator };
}

PreProcessorTokenStreamDiamond::Transition PreProcessorTokenStreamDiamond::OnOperatorAngleBracket() const
{
	if (ch.value == value[0])
		return { Action::Keep, State::OperatorWithEquals };
	else if (ch.value == '=')
		return { Action::Keep, PreProcessorTokenType::Operator };
	return { Action::PutBack, PreProcessorTokenType::Operator };
}

PreProcessorTokenStreamDiamond::Transition PreProcessorTokenStreamDiamond::OnMinus()
{
	if (ch.value == '-' || ch.value == '=')
		return { Action::Keep, PreProcessorTokenType::Operator };
	else if (ch.value == '>')
	{
		auto peek = stream.GetNext();
		if (peek.value == '*')
		{
			value += ch.value;
			ch = peek;
		}
		else if (peek.value != 0)
		{
			stream.PutBack(peek);
		}
		return { Action::Keep, PreProcessorTokenType::Operator };
	}
	return { Action::PutBack, PreProcessorTokenType::Operator };
}

PreProcessorTokenStreamDiamond::Transition PreProcessorTokenStreamDiamond::OnDot()
{
	if (ch.value == '*')
		return { Action::Keep, PreProcessorTokenType::Operator };
	else if (ch.value == '.')
	{
		auto peek = stream.GetNext();
		if (peek.value == '.')
		{
			value += peek.value;
			return { Action::Keep, PreProcessorTokenType::Operator };
		}
		if (peek.value != 0)
			stream.PutBack(peek);
	}
	return { Action::PutBack, PreProcessorTokenType::Operator };
}

