#pragma once

#include "raylib.h"
#include <memory>
#include "Item.h"

namespace BoxConstants {
	constexpr int kWidth = 150;
	constexpr int kHeight = 100;
} // BoxConstants

class InputBox {
public:
	InputBox(float x, float y);

	bool IsItemTouching(Item *item);

	void SnapItemInBox(Item *item);

	void RemoveItem();

	Item* GetHeldItem();

	Rectangle GetRect();

	Vector2 GetPosition();

private:
    Rectangle m_box;

    Item *m_heldItem;

    Vector2 m_position;
};