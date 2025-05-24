#pragma once

#include <array>
#include <map>
#include <memory>
#include <string>

#include "Item.h"

// Should never intake Item objects, only take in ItemType.
namespace ItemFactory {
const inline std::map<ItemType, std::string> names{
    {ItemType::NONE, "None"},
    {ItemType::STAR, "Star"},
    {ItemType::ITEM, "Item"},
    {ItemType::OUTPUT, "Output"}};

const inline std::array<ItemType, ItemType::NONE> inputs{ItemType::STAR,
                                                         ItemType::ITEM};

const inline std::array<ItemType, ItemType::NONE> outputs{ItemType::OUTPUT};

const inline std::map<ItemType, std::string> textureFiles{
    {ItemType::NONE, "resources/textures/none.png"},
    {ItemType::STAR, "resources/textures/new star.png"},
    {ItemType::ITEM, "resources/textures/star.png"},
    {ItemType::OUTPUT, "resources/textures/none.png"}};

std::shared_ptr<Item> CreateItemFromType(ItemType type);

std::shared_ptr<Item> CreateItemFromTypePos(ItemType type, float x, float y);

bool IsOutputType(ItemType type);

bool IsInputType(ItemType type);
};  // namespace ItemFactory