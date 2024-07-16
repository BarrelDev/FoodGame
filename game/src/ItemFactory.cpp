#include "ItemFactory.h"

#include <iostream>

std::shared_ptr<Item> ItemFactory::CreateItemFromType(ItemType type) 
{
  std::shared_ptr<Item> output = std::make_shared<Item>(
      names.at(type), type, 0.f, 0.f, textureFiles.at(type));

  return output;
}

std::shared_ptr<Item> ItemFactory::CreateItemFromTypePos(ItemType type, float x, float y) 
{
  std::shared_ptr<Item> output = std::make_shared<Item>(
      names.at(type), type, x, y, textureFiles.at(type));

  return output;
}