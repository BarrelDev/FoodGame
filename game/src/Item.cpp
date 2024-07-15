#include "Item.h"

Item::Item(std::string name, float x, float y,
           const char *textureFilePath)
    : m_position{x, y}, m_image{LoadImage(textureFilePath)},
      m_rect{0.f, 0.f, (float)m_image.width, (float)m_image.height},
      m_center{(float)m_image.width / 2.f,
               (float)m_image.height / 2.f},
      m_name{name}, m_id{id++} {}

Vector2 Item::GetPosition() { return m_position; }

void Item::SetX(float x) { m_position.x = x; }

void Item::SetY(float y) { m_position.y = y; }

Image Item::GetImage() { return ImageCopy(m_image); }

Rectangle Item::GetRect() { return m_rect; }

Vector2 Item::GetCenter() { return m_center; }

float Item::GetWidth() { return m_image.width; }

float Item::GetHeight() { return m_image.height; }

std::string Item::GetName() { return m_name;  }

int Item::GetID() { return m_id; }