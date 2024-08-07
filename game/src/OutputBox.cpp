#include "OutputBox.h"

#include <iostream>

#include "ItemFactory.h"

OutputBox::OutputBox(float x, float y, std::shared_ptr<InputBox> left,
                     std::shared_ptr<InputBox> right)
    : m_position{x, y},
      m_box{x, y, OutputBoxConstants::kWidth, OutputBoxConstants::kHeight},
      m_leftInput{left},
      m_rightInput{right},
      m_heldItem{nullptr} {}

void OutputBox::RemoveItem() { m_heldItem = nullptr; }

std::shared_ptr<Item> OutputBox::GetOutputItem() {
  std::shared_ptr<Item> leftItem = m_leftInput->GetHeldItem();
  std::shared_ptr<Item> rightItem = m_rightInput->GetHeldItem();

  if (leftItem != nullptr && rightItem != nullptr) {
    auto input = std::pair<ItemType, ItemType>{leftItem->GetType(),
                                               rightItem->GetType()};

    auto it = recipies.find(input);

    if (it == recipies.end()) return nullptr;

    ItemType outType = it->second;

    if (m_heldItem != nullptr && outType == m_heldItem->GetType())
      return m_heldItem;

    std::shared_ptr<Item> output = ItemFactory::CreateItemFromTypePos(
        outType, m_position.x + OutputBoxConstants::kWidth / 2.0f,
        m_position.y + OutputBoxConstants::kHeight / 2.0f);

    m_heldItem = output;

    return output;
  }
  return nullptr;
}

bool OutputBox::IsPairInMap(ItemType one, ItemType two) {
  auto input = std::pair<ItemType, ItemType>{one, two};
  auto it = recipies.find(input);
  if (it == recipies.end()) return false;

  return true;
}

Rectangle OutputBox::GetRect() { return m_box; }

Vector2 OutputBox::GetPosition() { return m_position; }