#pragma once
#include <string>
#include "Char.h"
#include "Position.h"
#include "PreProcessorToken.h"
#include "PreProcessorTokenStream.h"
#include "CharStream.h"

class PreProcessorTokenStreamCpp : public PreProcessorTokenStream
{
public:
	PreProcessorTokenStreamCpp() = delete;
	PreProcessorTokenStreamCpp(CharStream& stream);
	PreProcessorTokenStreamCpp(const PreProcessorTokenStreamCpp& rhs) = delete;
	~PreProcessorTokenStreamCpp() noexcept = default;

	PreProcessorTokenStreamCpp& operator=(const PreProcessorTokenStreamCpp& rhs) = delete;

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
	Transition OnOperatorWithEquals() const;
	Transition OnOperatorSelfOrEquals() const;
	Transition OnOperatorDouble() const;
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
		OperatorWithEquals,
		OperatorSelfOrEquals,
		OperatorDouble,
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

