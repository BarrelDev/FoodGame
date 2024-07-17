#pragma once

#include "raylib.h"
#include <memory>
#include <map>
#include <utility>

#include "Item.h"
#include "InputBox.h"

namespace OutputBoxConstants {
	constexpr int kWidth = 150;
	constexpr int kHeight = 100;
} // namespace BoxConstants

class OutputBox 
{
public:
  OutputBox(float x, float y, std::shared_ptr<InputBox> left, std::shared_ptr<InputBox> right);

  void RemoveItem();

  inline std::shared_ptr<Item> GetHeldItem() { return m_heldItem; };

  Rectangle GetRect();

  Vector2 GetPosition();

  std::shared_ptr<Item> GetOutputItem();

private:
  Rectangle m_box;

  std::shared_ptr<Item> m_heldItem;

  std::shared_ptr<InputBox> m_leftInput;
  std::shared_ptr<InputBox> m_rightInput;

  Vector2 m_position;

  const static inline std::map<std::pair<ItemType, ItemType>, ItemType> recipies{
	  {{ItemType::STAR, ItemType::ITEM}, ItemType::OUTPUT},
      {{ItemType::ITEM, ItemType::STAR}, ItemType::OUTPUT}
  };
};