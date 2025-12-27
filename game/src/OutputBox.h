#pragma once

#include <map>
#include <memory>
#include <utility>

#include "InputBox.h"
#include "Item.h"
#include "raylib.h"

namespace OutputBoxConstants {
constexpr int kWidth = 240;
constexpr int kHeight = 160;
}  // namespace OutputBoxConstants

class OutputBox {
 public:
  OutputBox(float x, float y, std::shared_ptr<InputBox> left,
            std::shared_ptr<InputBox> right);

  void RemoveItem();

  inline std::shared_ptr<Item> GetHeldItem() { return m_heldItem; };

  Rectangle GetRect();

  Vector2 GetPosition();

  std::shared_ptr<Item> GetOutputItem();

  static bool IsPairInMap(ItemType one, ItemType two);

 private:
  Rectangle m_box;

  std::shared_ptr<Item> m_heldItem;

  std::shared_ptr<InputBox> m_leftInput;
  std::shared_ptr<InputBox> m_rightInput;

  Vector2 m_position;

  const static inline std::map<std::pair<ItemType, ItemType>, ItemType>
      recipies{{{ItemType::STAR, ItemType::ITEM}, ItemType::OUTPUT},
               {{ItemType::ITEM, ItemType::STAR}, ItemType::OUTPUT},
               {{ItemType::NONE, ItemType::NONE}, ItemType::NONE}};
};