#pragma once

#include <memory>
#include <map>
#include <string>

#include "raylib.h"

#include "Item.h"

namespace ItemFactory {
	const inline std::map<ItemType, std::string> names{
		{ItemType::NONE, "None"},
		{ItemType::STAR, "Star"},
		{ItemType::ITEM, "Item"},
		{ItemType::OUTPUT, "Output"}
	};

	const inline std::map<ItemType, const char *> textureFiles{
        {ItemType::NONE, (const char*) "resources/textures/none.png"},
        {ItemType::STAR, (const char*) "resources/textures/new star.png"},
        {ItemType::ITEM, (const char*) "resources/textures/star.png"},
		{ItemType::OUTPUT, (const char*) "resources/textures/none.png"}
	};

	std::shared_ptr<Item> CreateItemFromType(ItemType type);

	std::shared_ptr<Item> CreateItemFromTypePos(ItemType type, float x, float y);
};