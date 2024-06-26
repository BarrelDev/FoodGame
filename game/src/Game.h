#pragma once

#include "raylib.h"
#include <vector>

#include "Constants.h"
#include "Item.h"
#include "InputBox.h"

// Store ball shape position
Vector2 ballPosition{ RenderConstants::kScreenWidth / 2.0f, RenderConstants::kScreenHeight / 4.0f };

// Create Items with Images
Item starItem{ "Star", 0, (float)RenderConstants::kScreenWidth / 6.0f, 200, "resources/textures/new star.png" };
Item testItem{ "Item", 1, (float)RenderConstants::kScreenWidth / 2.0f, 3.0f * (float)(RenderConstants::kScreenHeight / 4), "resources/textures/star.png" };

std::vector<Item*> items;
std::vector<Texture2D> textures;

// Collision for items.
Rectangle inputOne{ 
	(float) RenderConstants::kScreenWidth / 12.0f,
	(float) RenderConstants::kScreenHeight / 8.0f,
	BoxConstants::kBoxWidth, BoxConstants::kBoxHeight
};
Rectangle inputTwo{ 
	(float)RenderConstants::kScreenWidth / 2.5f, 
	(float)RenderConstants::kScreenHeight / 8.0f, 
	BoxConstants::kBoxWidth, BoxConstants::kBoxHeight 
};

InputBox leftInput{(float)RenderConstants::kScreenWidth / 12.0f,
                   (float)RenderConstants::kScreenHeight / 8.0f};

InputBox rightInput{(float)RenderConstants::kScreenWidth / 2.5f,
                    (float)RenderConstants::kScreenHeight / 8.0f};



Rectangle output{ 
	(float)RenderConstants::kScreenWidth / 1.4f, 
	(float)RenderConstants::kScreenHeight / 8.0f, 
	BoxConstants::kBoxWidth, 
	BoxConstants::kBoxHeight 
};
Rectangle optionOne;
Rectangle optionTwo;
Rectangle optionThree;

// Store mouse position
Vector2 mousePos;

bool isHolding;

Item *heldItem;

bool isLeftClicking;
bool isRightClicking;

void main();