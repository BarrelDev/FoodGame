#include "OptionBox.h"

#include <raylib.h>

#include <__msvc_ostream.hpp>
#include <iostream>
#include <memory>
#include <random>

#include "Item.h"
#include "ItemFactory.h"

OptionBox::OptionBox(float x, float y)
    : m_position{x, y},
      m_box{x, y, OptionBoxConstants::kWidth, OptionBoxConstants::kHeight},
      m_heldItem{nullptr} {}

bool OptionBox::IsItemTouching(std::shared_ptr<Item> item) {
  if (item == nullptr) return false;

  Vector2 position{item->GetPosition()};
  Vector2 center{item->GetCenter()};
  Rectangle rect{item->GetRect()};

  if (CheckCollisionRecs(m_box, {position.x - center.x, position.y - center.y,
                                 rect.width, rect.height})) {
    return true;
  }
  return false;
}

void OptionBox::SpawnItemInBox() {
  m_heldItem = nullptr;

  std::random_device rd;
  std::uniform_int_distribution<int> dist(
      0, static_cast<int>(ItemFactory::inputs.size()) - 1);

  // std::cout << dist(rd) << std::endl;

  auto item = ItemFactory::CreateItemFromTypePos(
      ItemFactory::inputs.at(dist(rd)),
      m_position.x + OptionBoxConstants::kWidth,
      m_position.y + OptionBoxConstants::kHeight);
  item->SetX(m_position.x + OptionBoxConstants::kWidth / 2.0f);
  item->SetY(m_position.y + OptionBoxConstants::kHeight / 2.0f);
  m_heldItem = item;
}

void OptionBox::RemoveItem() { m_heldItem = nullptr; }

Rectangle OptionBox::GetRect() const { return m_box; }

Vector2 OptionBox::GetPosition() const { return m_position; }