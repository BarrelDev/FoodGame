#include "TextureManager.h"

Texture2D TextureManager::GetTextureFromItemType(ItemType type) {
  if (IsWindowReady()) {
    if (itemTextures.contains(type)) {
      return itemTextures.find(type)->second;
    } else {
      return Texture2D{};
    }
  } else {
    throw "window is not initialized yet";
  }
}

void TextureManager::UpdateItemTextures(std::shared_ptr<Item> item) {
  if (!itemTextures.contains(item->GetType()) && IsWindowReady()) {
    auto texture = LoadTextureFromImage(item->GetImage());
    itemTextures.insert({item->GetType(), texture});
  }
}

void TextureManager::UnloadItemTextures() {
  if (IsWindowReady()) {
    for (const auto& pair : itemTextures) {
      UnloadTexture(pair.second);
    }
  }
}