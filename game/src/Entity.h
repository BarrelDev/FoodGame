#pragma once

#include <bitset>

#include "Constants.h"

typedef unsigned int EntityIndex;
typedef unsigned int EntityVersion;
typedef unsigned long long EntityID;

#define INVALID_ENTITY CreateEntityId(EntityIndex(-1), 0)

// In order to effectively delete entities from a scene, both an EntityVersion and EntityIndex are assigned to an ID.
// The top 32 bits of the EntityID is the EntityIndex and the bottom 32 bits of the EntityID is the EntityVersion.

inline EntityID CreateEntityId(EntityIndex index, EntityVersion version)
{
	// Shift the index up 32, and put the version in the bottom
	return ((EntityID)index << 32) | ((EntityID)version);
}
inline EntityIndex GetEntityIndex(EntityID id)
{
	// Shift down 32 so we lose the version and get our index
	return id >> 32;
}
inline EntityVersion GetEntityVersion(EntityID id)
{
	// Cast to a 32 bit int to get our version number (loosing the top 32 bits)
	return (EntityVersion)id;
}
inline bool IsEntityValid(EntityID id)
{
	// Check if the index is our invalid index
	return (id >> 32) != EntityIndex(-1);
}