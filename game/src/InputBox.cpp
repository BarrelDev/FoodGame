#include "InputBox.h"

InputBox::InputBox(float x, float y)
    : m_position{x, y}, m_box{x, y, BoxConstants::kWidth,
                              BoxConstants::kHeight},
      m_heldItem{nullptr} {}

bool InputBox::IsItemTouching(std::shared_ptr<Item> item) 
{
  if (item == nullptr)
    return false;

  Vector2 position{item->GetPosition()};
  Vector2 center{item->GetCenter()};
  Rectangle rect{item->GetRect()};

  if (CheckCollisionRecs(m_box, {position.x - center.x,
                                    position.y - center.y,
                                    rect.width, rect.height})) {
    return true;
  }
  return false;
}

void InputBox::SnapItemInBox(std::shared_ptr<Item>  item) { 
  if (item != nullptr) {
    item->SetX(m_position.x + BoxConstants::kWidth / 2.0f);
    item->SetY(m_position.y + BoxConstants::kHeight / 2.0f);
    m_heldItem = item;
  }
}

void InputBox::RemoveItem() { m_heldItem = nullptr; }

Rectangle InputBox::GetRect() { return m_box; }

Vector2 InputBox::GetPosition() { return m_position; }