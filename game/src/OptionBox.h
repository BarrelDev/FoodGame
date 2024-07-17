#pragma once

#include <memory>

#include "Item.h"
#include "ItemFactory.h"
#include "raylib.h"

namespace OptionBoxConstants {
constexpr int kWidth = 150;
constexpr int kHeight = 100;
}  // namespace OptionBoxConstants

class OptionBox {
 public:
  OptionBox(float x, float y);

  bool IsItemTouching(std::shared_ptr<Item> item);

  void SpawnItemInBox();  // Spawn a random item into the option box and
                          // set it as the held item.

  void RemoveItem();

  inline bool IsHoldingItem() { return m_heldItem != nullptr; };

  inline std::shared_ptr<Item> GetHeldItem() { return m_heldItem; };

  Rectangle GetRect();

  Vector2 GetPosition();

 private:
  Rectangle m_box;

  std::shared_ptr<Item> m_heldItem;

  Vector2 m_position;

  friend class OutputBox;

  friend class InputBox;
};