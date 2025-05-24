#include "ItemFactory.h"

#include <memory>

#include "Item.h"

std::shared_ptr<Item> ItemFactory::CreateItemFromType(ItemType type) {
  std::shared_ptr<Item> output = std::make_shared<Item>(
      names.at(type), type, 0.f, 0.f, textureFiles.at(type).c_str());

  return output;
}

std::shared_ptr<Item> ItemFactory::CreateItemFromTypePos(ItemType type, float x,
                                                         float y) {
  std::shared_ptr<Item> output = std::make_shared<Item>(
      names.at(type), type, x, y, textureFiles.at(type).c_str());

  return output;
}

bool ItemFactory::IsOutputType(ItemType type) {
  for (const auto& t : outputs) {
    if (t == type) {
      return true;
    }
  }

  return false;
}

bool ItemFactory::IsInputType(ItemType type) {
  for (const auto& t : inputs) {
    if (t == type) {
      return true;
    }
  }

  return false;
}