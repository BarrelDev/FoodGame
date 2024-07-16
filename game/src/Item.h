#pragma once

#include "raylib.h"

#include <string>

static int id = 0;

enum ItemType {
	NONE,
	STAR,
	ITEM
};

class Item
{
	public:
		Item();

		Item(std::string name, ItemType type, float x, float y, const char *textureFilePath);

		Vector2 GetPosition() const;

		void SetX(float x);

		void SetY(float y);

		Image GetImage() const;

		Rectangle GetRect() const;

		Vector2 GetCenter() const;

		int GetWidth() const;

		int GetHeight() const;

		ItemType GetType() const;

		std::string GetName() const;

		int GetID() const;
	private:
		Vector2 m_position;

		Image m_image;

		Rectangle m_rect;

		Vector2 m_center;

		ItemType m_type{ ItemType::NONE };

		std::string m_name;

		int m_id;
};
