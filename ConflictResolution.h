#pragma once
#include <string>

enum class ConflictResolution
{
	None,
	KeepOld,
	KeepNew
};

std::string ToString(ConflictResolution value);

