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

	bool IsItemTouching(std::shared_ptr<Item> item);

	void SnapItemInBox(std::shared_ptr<Item> item);

	void RemoveItem();

	std::shared_ptr<Item> GetHeldItem();

	Rectangle GetRect();

	Vector2 GetPosition();

private:
    Rectangle m_box;

    std::shared_ptr<Item> m_heldItem;

    Vector2 m_position;
};