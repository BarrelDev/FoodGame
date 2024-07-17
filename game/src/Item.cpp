#include "Item.h"

Item::Item()
    : m_position{0.f, 0.f},
      m_image{ImageText("NONE", 16, BLACK)},
      m_rect{0.f, 0.f, 0.f, 0.f},
      m_center{0.f, 0.f},
      m_name{"NONE"},
      m_id{id++} {}

Item::Item(std::string name, ItemType type, float x, float y,
           const char *textureFilePath)
    : m_position{x, y},
      m_image{LoadImage(textureFilePath)},
      m_rect{0.f, 0.f, (float)m_image.width, (float)m_image.height},
      m_center{(float)m_image.width / 2.f, (float)m_image.height / 2.f},
      m_name{name},
      m_type{type},
      m_id{id++} {}

Vector2 Item::GetPosition() const { return m_position; }

void Item::SetX(float x) { m_position.x = x; }

void Item::SetY(float y) { m_position.y = y; }

Image Item::GetImage() const { return ImageCopy(m_image); }

Rectangle Item::GetRect() const { return m_rect; }

Vector2 Item::GetCenter() const { return m_center; }

int Item::GetWidth() const { return m_image.width; }

int Item::GetHeight() const { return m_image.height; }

ItemType Item::GetType() const { return m_type; }

std::string Item::GetName() const { return m_name; }

int Item::GetID() const { return m_id; }