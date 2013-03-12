#include "TransitionType.h"
#include <stdexcept>

std::string ToString(TransitionType value)
{
	switch(value)
	{
	case TransitionType::Shift:
		return "Shift";
	case TransitionType::Reduce:
		return "Reduce";
	case TransitionType::Accept:
		return "Accept";
	}
	throw std::logic_error("Invalid transition type.");
}

