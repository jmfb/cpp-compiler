#include "ConflictResolution.h"
#include <stdexcept>

std::string ToString(ConflictResolution value)
{
	switch(value)
	{
	case ConflictResolution::None:
		return "None";
	case ConflictResolution::KeepOld:
		return "KeepOld";
	case ConflictResolution::KeepNew:
		return "KeepNew";
	}
	throw std::logic_error("Invalid conflict resolution.");
}

