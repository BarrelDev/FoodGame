#include "OutputBox.h"

OutputBox::OutputBox(float x, float y, std::shared_ptr<InputBox> left,
                     std::shared_ptr<InputBox> right)
    : m_position{x, y},
      m_box{x, y, OutputBoxConstants::kWidth, OutputBoxConstants::kHeight},
      m_leftInput{left}, m_rightInput {right},
      m_heldItem{nullptr} {}

void OutputBox::RemoveItem() { m_heldItem = nullptr; }

std::shared_ptr<Item> OutputBox::GetHeldItem() { return m_heldItem; }

std::shared_ptr<Item> OutputBox::GetOutputItem() { 
  auto leftItem = m_leftInput->GetHeldItem();
  auto rightItem = m_rightInput->GetHeldItem();

  return rightItem;
}

Rectangle OutputBox::GetRect() { return m_box; }

Vector2 OutputBox::GetPosition() { return m_position; }