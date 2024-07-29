#pragma once

#include <map>
#include <memory>
#include <string>

#include "Item.h"
#include "raylib.h"

namespace TextureManager {
inline std::map<ItemType, Texture2D> itemTextures;

Texture2D GetTextureFromItemType(ItemType type);

void UpdateItemTextures(std::shared_ptr<Item> item);

void UnloadItemTextures();
};  // namespace TextureManager