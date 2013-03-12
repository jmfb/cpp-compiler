#pragma once
#include "ConflictResolution.h"
#include "Terminal.h"
#include "TransitionAction.h"
#include "TransitionType.h"

class ConflictResolver
{
public:
	ConflictResolver() = default;
	ConflictResolver(const ConflictResolver& rhs) = delete;
	~ConflictResolver() = default;

	ConflictResolver& operator=(const ConflictResolver& rhs) = delete;

	virtual ConflictResolution Resolve(
		const Terminal& terminal,
		const TransitionAction& oldAction,
		const TransitionAction& newAction) = 0;

protected:
	static ConflictResolution ChooseResolution(
		TransitionType oldType,
		TransitionType newType,
		TransitionType keepType);
};

