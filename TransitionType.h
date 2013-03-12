#pragma once
#include <string>

enum class TransitionType
{
	Shift,
	Reduce,
	Accept
};

std::string ToString(TransitionType value);

