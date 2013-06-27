#pragma once
#include <string>
#include "Char.h"
#include "Position.h"
#include "PreProcessorToken.h"
#include "PreProcessorTokenStream.h"
#include "CharStream.h"

class PreProcessorTokenStreamDiamond : public PreProcessorTokenStream
{
public:
	PreProcessorTokenStreamDiamond() = delete;
	PreProcessorTokenStreamDiamond(CharStream& stream);
	PreProcessorTokenStreamDiamond(const PreProcessorTokenStreamDiamond& rhs) = delete;
	~PreProcessorTokenStreamDiamond() noexcept = default;

	PreProcessorTokenStreamDiamond& operator=(const PreProcessorTokenStreamDiamond& rhs) = delete;

	PreProcessorToken GetNextToken() final;

private:
	class Transition;

	Transition StateDispatch();

	PreProcessorToken GetTokenBeforeEndOfFile() const;

	Transition OnInitial();
	Transition OnWhitespace() const;
	Transition OnForwardSlash() const;
	Transition OnSingleLineComment() const;
	Transition OnMultiLineComment() const;
	Transition OnMultiLineCommentStar() const;
	Transition OnIdentifier() const;
	Transition OnString() const;
	Transition OnStringEscape() const;
	Transition OnCharacter() const;
	Transition OnCharacterEscape() const;
	Transition OnNumber() const;
	Transition OnOperator() const;
	Transition OnOperatorColon() const;
	Transition OnOperatorQuestion();
	Transition OnOperatorWithEquals() const;
	Transition OnOperatorSelfOrEquals() const;
	Transition OnOperatorAngleBracket() const;
	Transition OnMinus();
	Transition OnDot();

	enum class State
	{
		Initial,
		Whitespace,
		ForwardSlash,
		SingleLineComment,
		MultiLineComment,
		MultiLineCommentStar,
		Identifier,
		String,
		StringEscape,
		Character,
		CharacterEscape,
		Number,
		OperatorColon,
		OperatorQuestion,
		OperatorWithEquals,
		OperatorSelfOrEquals,
		OperatorAngleBracket,
		Minus,
		Dot
	};

private:
	State state = State::Initial;
	Position position;
	std::string value;
	Char ch;
};

