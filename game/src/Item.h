#pragma once

#include "raylib.h"

#include <string>

class Item
{
	public:
		Item(std::string name, int id, float x, float y, const char *textureFilePath);

		Vector2 GetPosition();

		void SetX(float x);

		void SetY(float y);

		Image GetImage();

		Rectangle GetRect();

		Vector2 GetCenter();

		float GetWidth();

		float GetHeight();

		std::string GetName();

		int GetID();
	private:
		Vector2 m_position;

		Image m_image;

		Rectangle m_rect;

		Vector2 m_center;

		std::string m_name;

		int m_id;
};
