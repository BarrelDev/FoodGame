#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "Constants.h"
#include "InputBox.h"
#include "Item.h"
#include "ItemFactory.h"
#include "OptionBox.h"
#include "OutputBox.h"
#include "raylib.h"
#include "TextureManager.h"

// Store ball shape position
Vector2 ballPosition{RenderConstants::kScreenWidth / 2.0f,
                     RenderConstants::kScreenHeight / 4.0f};

// Create Items with Images
std::shared_ptr<Item> starItem = ItemFactory::CreateItemFromTypePos(
    ItemType::STAR, (float)RenderConstants::kScreenWidth / 6.0f, 200.0f);

std::shared_ptr<Item> testItem = ItemFactory::CreateItemFromTypePos(
    ItemType::ITEM, (float)RenderConstants::kScreenWidth / 2.0f,
    3.0f * (float)(RenderConstants::kScreenHeight / 4));

// Store items and their respective textures dynamically for easy access.
std::vector<std::shared_ptr<Item>> items;

// Collision for items.
std::shared_ptr<InputBox> leftInput{
    std::make_shared<InputBox>((float)RenderConstants::kScreenWidth / 12.0f,
                               (float)RenderConstants::kScreenHeight / 8.0f)};

std::shared_ptr<InputBox> rightInput{
    std::make_shared<InputBox>((float)RenderConstants::kScreenWidth / 2.5f,
                               (float)RenderConstants::kScreenHeight / 8.0f)};

OutputBox outputBox{(float)RenderConstants::kScreenWidth / 1.4f,
                    (float)RenderConstants::kScreenHeight / 8.0f, leftInput,
                    rightInput};

OptionBox optionBox_left{(float)RenderConstants::kScreenWidth / 12.0f,
                         (float)RenderConstants::kScreenHeight / 1.5f};

OptionBox optionBox_center{(float)RenderConstants::kScreenWidth / 2.5f,
                           (float)RenderConstants::kScreenHeight / 1.5f};

OptionBox optionBox_right{(float)RenderConstants::kScreenWidth / 1.4f,
                          (float)RenderConstants::kScreenHeight / 1.5f};

// Temporary Rects for storing item spawn boxes.
Rectangle optionOne;
Rectangle optionTwo;
Rectangle optionThree;

// Store mouse position
Vector2 mousePos;

bool isHolding;
std::shared_ptr<Item> heldItem;

std::shared_ptr<Item> outputItem;

bool isLeftClicking;
bool isRightClicking;

int main();