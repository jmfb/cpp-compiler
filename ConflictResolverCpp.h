#pragma once
#include "ConflictResolver.h"

class ConflictResolverCpp : public ConflictResolver
{
public:
	ConflictResolverCpp() = default;
	ConflictResolverCpp(const ConflictResolverCpp& rhs) = default;
	~ConflictResolverCpp() = default;

	ConflictResolverCpp& operator=(const ConflictResolverCpp& rhs) = default;

	ConflictResolution Resolve(
		const Terminal& terminal,
		const TransitionAction& oldAction,
		const TransitionAction& newAction) final;
};

