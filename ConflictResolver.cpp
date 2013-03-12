#include "ConflictResolver.h"

ConflictResolution ConflictResolver::ChooseResolution(
	TransitionType oldType,
	TransitionType newType,
	TransitionType keepType)
{
	if (oldType == newType)
		return ConflictResolution::None;
	if (oldType == keepType)
		return ConflictResolution::KeepOld;
	if (newType == keepType)
		return ConflictResolution::KeepNew;
	return ConflictResolution::None;
}

