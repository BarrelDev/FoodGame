#pragma once

#include "raylib.h"

#include "Constants.h"
#include "Item.h"

// Store ball shape position
Vector2 ballPosition{ RenderConstants::kScreenWidth / 2.0f, RenderConstants::kScreenHeight / 4.0f };

// Create Items with Images
Item starItem{ "Star", 0, (float)RenderConstants::kScreenWidth / 6.0f, 200, "resources/textures/new star.png" };
Item testItem{ "Item", 0, (float)RenderConstants::kScreenWidth / 2.0f, 3.0f * (float)(RenderConstants::kScreenHeight / 4), "resources/textures/star.png" };

// Create Positioning Vector and Rectangle for starItem
Rectangle starRect{ 0.f, 0.f, (float)starItem.GetImage().width, (float)starItem.GetImage().height };
Vector2 starCenter{ (float)starItem.GetImage().width / 2.f, (float)starItem.GetImage().height / 2.f };

// Store mouse position
Vector2 mousePos;

bool isHolding;

void main();