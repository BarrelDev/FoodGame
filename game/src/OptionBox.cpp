#include "OptionBox.h"

#include <cstdlib>
#include <ctime>

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
  std::srand(std::time(nullptr));

  auto item = ItemFactory::CreateItemFromTypePos(
      static_cast<ItemType>(std::rand() % NONE),
      m_position.x + OptionBoxConstants::kWidth,
      m_position.y + OptionBoxConstants::kHeight);
  item->SetX(m_position.x + OptionBoxConstants::kWidth / 2.0f);
  item->SetY(m_position.y + OptionBoxConstants::kHeight / 2.0f);
  m_heldItem = item;
}

void OptionBox::RemoveItem() { m_heldItem = nullptr; }

Rectangle OptionBox::GetRect() { return m_box; }

Vector2 OptionBox::GetPosition() { return m_position; }