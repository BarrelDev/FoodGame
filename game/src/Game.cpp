#include "Game.h"

#include <raylib.h>

#include <__msvc_ostream.hpp>
#include <iostream>
#include <memory>

#include "Constants.h"
#include "Item.h"
#include "OutputBox.h"
#include "TextureManager.h"

int main() {
  std::cout << "starting" << std::endl;
  // Start Rendering Window
  InitWindow(RenderConstants::kScreenWidth, RenderConstants::kScreenHeight,
             "raylib test");

  SetTargetFPS(RenderConstants::kTargetFPS);

  do {
    optionBox_left.SpawnItemInBox();
    optionBox_center.SpawnItemInBox();
    optionBox_right.SpawnItemInBox();
  } while (!IsValidOptionCombination(optionBox_left.GetHeldItem()->GetType(),
                                     optionBox_center.GetHeldItem()->GetType(),
                                     optionBox_right.GetHeldItem()->GetType()));

  Texture2D addIcon{LoadTexture("resources/textures/plus.png")};
  Texture2D equalIcon{LoadTexture("resources/textures/equals.png")};

  items.push_back(optionBox_left.GetHeldItem());
  items.push_back(optionBox_center.GetHeldItem());
  items.push_back(optionBox_right.GetHeldItem());

  for (auto item : items) {
    std::cout << item->GetID() << ":" << item->GetName() << std::endl;
    TextureManager::UpdateItemTextures(item);
  }

  // Main Game Loop
  while (!WindowShouldClose()) {
    mousePos = GetMousePosition();
    // Check if mouse button held
    if (IsMouseButtonDown(0))
      isLeftClicking = true;
    else if (IsMouseButtonReleased(0))
      isLeftClicking = false;

    // Check if item is in range of being held by mouse cursor.
    if (isLeftClicking) {
      Vector2 itemPos;
      Vector2 itemEdge;
      for (int i = 0; i < items.size(); ++i) {
        std::shared_ptr<Item> item = items[i];

        itemPos = Vector2{item->GetPosition().x - item->GetWidth() / 2.0f,
                          item->GetPosition().y - item->GetHeight() / 2.0f};
        itemEdge = Vector2{item->GetPosition().x + item->GetWidth() / 2.0f,
                           item->GetPosition().y + item->GetHeight() / 2.0f};
        if (mousePos.x >= itemPos.x && mousePos.x <= itemEdge.x &&
            mousePos.y >= itemPos.y && mousePos.y <= itemEdge.y) {
          isHolding = true;
          heldItem = item;
        }
      }
    } else {
      isHolding = false;
    }

    // Move heldItem to mouse position if left mouse button held
    if (isHolding) {
      if (mousePos.x <=
              (float)RenderConstants::kScreenWidth - heldItem->GetCenter().x &&
          mousePos.x >= heldItem->GetCenter().x)
        heldItem->SetX(mousePos.x);
      if (mousePos.y <=
              (float)RenderConstants::kScreenHeight - heldItem->GetCenter().y &&
          mousePos.y >= heldItem->GetCenter().y)
        heldItem->SetY(mousePos.y);
    } else if (heldItem != nullptr) {
      if (leftInput->IsItemTouching(heldItem) && !leftInput->IsHoldingItem()) {
        leftInput->SnapItemInBox(heldItem);
        heldItem = nullptr;
      } else if (leftInput->GetHeldItem() == heldItem) {
        leftInput->RemoveItem();
      }
      if (rightInput->IsItemTouching(heldItem) &&
          !rightInput->IsHoldingItem()) {
        rightInput->SnapItemInBox(heldItem);
        heldItem = nullptr;
      } else if (rightInput->GetHeldItem() == heldItem) {
        rightInput->RemoveItem();
      }
    }

    // Get the correct output item and load its data.

    outputItem = outputBox.GetOutputItem();

    // Display the possible output

    if (outputItem != nullptr) {
      TextureManager::UpdateItemTextures(outputItem);
      items.push_back(outputItem);

      // Delete the input items once the output has been picked up.
      if (heldItem == outputItem && leftInput->IsHoldingItem() &&
          rightInput->IsHoldingItem()) {
        DestroyItem(leftInput->GetHeldItem());
        leftInput->RemoveItem();
        DestroyItem(rightInput->GetHeldItem());
        rightInput->RemoveItem();

        score++;
      }
    }

    // Render Frame
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawText(TextFormat("Score: %04i", score), 10, 10, 40, BLACK);

    DrawText(TextFormat("%02i:%02i", 0, 0), RenderConstants::kScreenWidth - 110,
             10, 40, BLACK);

    DrawRectangleRec(leftInput->GetRect(), BLACK);

    DrawRectangleRec(rightInput->GetRect(), BLACK);

    DrawRectangleRec(outputBox.GetRect(), BLUE);

    DrawRectangleRec(optionBox_left.GetRect(), RED);

    DrawRectangleRec(optionBox_center.GetRect(), RED);

    DrawRectangleRec(optionBox_right.GetRect(), RED);

    DrawTexture(addIcon, RenderConstants::kScreenWidth / 3.65f,
                RenderConstants::kScreenHeight / 8.0f, WHITE);

    DrawTexture(equalIcon, RenderConstants::kScreenWidth / 1.7f,
                RenderConstants::kScreenHeight / 8.0f, WHITE);

    DrawText("FOOD GAME", 190, 200, 20, BLACK);

    // Render loaded items.

    for (auto item : items) {
      DrawTexturePro(TextureManager::GetTextureFromItemType(item->GetType()),
                     item->GetRect(),
                     {item->GetPosition().x, item->GetPosition().y,
                      item->GetRect().width, item->GetRect().height},
                     item->GetCenter(), 0, WHITE);

      DrawText(item->GetName().c_str(),
               (int)item->GetPosition().x + RenderConstants::kTextOffsetX,
               (int)item->GetPosition().y + RenderConstants::kTextOffsetY, 20,
               BLACK);
    }

    EndDrawing();
  }

  // Unload Textures
  TextureManager::UnloadItemTextures();

  CloseWindow();

  return 0;
}

bool IsValidOptionCombination(ItemType one, ItemType two, ItemType three) {
  if (OutputBox::IsPairInMap(one, two)) return true;
  if (OutputBox::IsPairInMap(one, three)) return true;
  if (OutputBox::IsPairInMap(two, three)) return true;

  return false;
}

bool DestroyItem(std::shared_ptr<Item> item) {
  auto it = std::find(items.begin(), items.end(), item);

  if (it == items.end()) return false;

  items.erase(it);
  return true;
}