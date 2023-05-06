#pragma once

#include <vector>
#include <iostream>

#include "Entity.h"
#include "Components.h"
#include "Constants.h"

struct Scene
{
	std::vector<ComponentPool> componentPools;
	std::vector<EntityIndex> freeEntities;

	struct EntityDesc
	{
		EntityID id;
		ComponentMask mask;
	};

	std::vector<EntityDesc> entities;

	EntityID NewEntity()
	{
		if (!freeEntities.empty())
		{
			EntityIndex newIndex = freeEntities.back();
			freeEntities.pop_back();
			EntityID newID = CreateEntityId(newIndex, GetEntityVersion(entities[newIndex].id));
			entities[newIndex].id = newID;
			return entities[newIndex].id;
		}
		entities.push_back({ CreateEntityId(EntityIndex(entities.size()), 0), ComponentMask() });
		return entities.back().id;
	}

	template<typename T>
	T* Assign(EntityID id)
	{
		int componentId = GetId<T>();

		if (componentPools.size() <= componentId) // Not enough component pool
		{
			componentPools.resize(componentId + 1, nullptr);
			//componentPools[componentId + 1] = nullptr;
		}
		if (componentPools[componentId] == nullptr) // New component, make a new pool
		{
			componentPools[componentId] = new ComponentPool(sizeof(T));
		}

		T* pComponent = new (componentPools[componentId].get(id)) T();

		entities[id].mask.set(componentId);
		return pComponent;
	}

	template<typename T>
	T* Get(EntityID id)
	{
		int componentId = GetId<T>();
		if (!entities[id].mask.test(componentId))
			return nullptr;

		T* pComponent = static_cast<T*>(componentPools[componentId]->get(id));
		return pComponent;
	}

	template<typename T>
	void Remove(EntityID id)
	{
		// ensures you're not accessing an entity that has been deleted
		if (entities[GetEntityIndex(id)].id != id)
			return;

		int componentId = GetId<T>();
		entities[GetEntityIndex(id)].mask.reset(componentId);
	}

	void DestroyEntity(EntityID id)
	{
		EntityID newID = CreateEntityId(EntityIndex(-1), GetEntityVersion(id) + 1);
		entities[GetEntityIndex(id)].id = newID;
		entities[GetEntityIndex(id)].mask.reset();
		freeEntities.push_back(GetEntityIndex(id));
	}
};

template<typename... ComponentTypes>
struct SceneView
{
	Scene* pScene{ nullptr };
	ComponentMask componentMask;
	bool all{ false };

	SceneView(Scene& scene) : pScene(&scene)
	{
		if (sizeof...(ComponentTypes) == 0)
		{
			all = true;
		}
		else
		{
			// Unpack the template parameters into an initializer list
			int componentIds[] = { 0, GetId<ComponentTypes>() ... };
			for (int i = 1; i < (sizeof...(ComponentTypes) + 1); i++)
				componentMask.set(componentIds[i]);
		}
	}

	struct Iterator
	{
		EntityIndex index;
		Scene* pScene;
		ComponentMask mask;
		bool all{ false };

		Iterator(Scene* pScene, EntityIndex index, ComponentMask mask, bool all)
			: pScene(pScene), index(index), mask(mask), all(all) {}

		EntityID operator*() const
		{
			return pScene->entities[index].id;
		}

		bool operator==(const Iterator& other) const
		{
			return index == other.index || index == pScene->entities.size();
		}

		bool operator!=(const Iterator& other) const
		{
			return index != other.index && index != pScene->entities.size();
		}

		bool ValidIndex()
		{
			return
				// It's a valid entity ID
				IsEntityValid(pScene->entities[index].id) &&
				// It has the correct component mask
				(all || mask == (mask & pScene->entities[index].mask));
		}

		Iterator& operator++()
		{
			do
			{
				index++;
			} while (index < pScene->entities.size() && !ValidIndex());
			return *this;
		}
	};

	const Iterator begin() const
	{
		int firstIndex = 0;
		while (firstIndex < pScene->entities.size() &&
			(componentMask != (componentMask & pScene->entities[firstIndex].mask)
				|| !IsEntityValid(pScene->entities[firstIndex].id)))
		{
			firstIndex++;
		}
		return Iterator(pScene, firstIndex, componentMask, all);
	}

	const Iterator end() const
	{
		return Iterator(pScene, EntityIndex(pScene->entities.size()), componentMask, all);
	}
};