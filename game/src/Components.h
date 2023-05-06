#pragma once

#include "raylib.h"

#include "Constants.h"

typedef std::bitset<MemoryConstants::kMaxComponents> ComponentMask;

// Generate Unique IDs for each component.
extern int s_componentCounter;
template <class T>
int GetId()
{
	static int s_componentId = s_componentCounter++;
	return s_componentId;
}

// Parent Component to inherit from to allow type organization.
struct Component {};

struct PositionComponent : Component
{
	Vector2 pos;
};

struct TextureComponent : Component
{
	const char* filePath;
	Image img{ nullptr };
};

struct ComponentPool
{
	char* pData{ nullptr };
	size_t elementSize{ 0 };

	ComponentPool(size_t element_size)
	{
		elementSize = element_size;
		pData = new char[elementSize * MemoryConstants::kMaxEntities];
	}

	~ComponentPool()
	{
		delete[] pData;
	}

	inline void* get(size_t index)
	{
		return pData + index * elementSize;
	}
};