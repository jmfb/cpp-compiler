#pragma once
#include "ConflictResolver.h"

class ConflictResolverDiamond : public ConflictResolver
{
public:
	ConflictResolverDiamond() = default;
	ConflictResolverDiamond(const ConflictResolverDiamond& rhs) = default;
	~ConflictResolverDiamond() = default;

	ConflictResolverDiamond& operator=(const ConflictResolverDiamond& rhs) = default;

	ConflictResolution Resolve(
		const Terminal& terminal,
		const TransitionAction& oldAction,
		const TransitionAction& newAction) final;
};

