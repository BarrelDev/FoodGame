#include "Item.h"

Item::Item(std::string name, int id, float x, float y, const char* textureFilePath) :
	m_position{ x, y }, m_image{ LoadImage(textureFilePath) }, m_name{ name }, m_id{id} {}

Vector2 Item::GetPosition() { return m_position; }

void Item::SetX(float x) { m_position.x = x; }

void Item::SetY(float y) { m_position.y = y; }

Image Item::GetImage() { return ImageCopy(m_image); }

std::string Item::GetName() { return m_name;  }

int Item::GetID() { return m_id; }