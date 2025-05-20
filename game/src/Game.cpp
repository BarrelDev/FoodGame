#include "Game.h"

#include <raylib.h>

#include <__msvc_ostream.hpp>
#include <iostream>
#include <memory>
#include <string>

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

  AddItem(optionBox_left.GetHeldItem());
  AddItem(optionBox_center.GetHeldItem());
  AddItem(optionBox_right.GetHeldItem());

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
        if (optionBox_left.GetHeldItem() == heldItem) {
          optionBox_left.RemoveItem();
        }
        if (optionBox_center.GetHeldItem() == heldItem) {
          optionBox_center.RemoveItem();
        }
        if (optionBox_right.GetHeldItem() == heldItem) {
          optionBox_right.RemoveItem();
        }
        heldItem = nullptr;
      } else if (leftInput->GetHeldItem() == heldItem) {
        leftInput->RemoveItem();
      }
      if (rightInput->IsItemTouching(heldItem) &&
          !rightInput->IsHoldingItem()) {
        rightInput->SnapItemInBox(heldItem);
        if (optionBox_left.GetHeldItem() == heldItem) {
          optionBox_left.RemoveItem();
        }
        if (optionBox_center.GetHeldItem() == heldItem) {
          optionBox_center.RemoveItem();
        }
        if (optionBox_right.GetHeldItem() == heldItem) {
          optionBox_right.RemoveItem();
        }
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
      AddItem(outputItem);

      // Delete the input items once the output has been picked up.
      if (heldItem == outputItem && leftInput->IsHoldingItem() &&
          rightInput->IsHoldingItem()) {
        DestroyItem(leftInput->GetHeldItem());
        leftInput->RemoveItem();
        DestroyItem(rightInput->GetHeldItem());
        rightInput->RemoveItem();

        holdingOutput = true;

        score++;
      }
    }

    if (holdingOutput && !isHolding) {
      holdingOutput = false;

      DestroyItem(heldItem);
      outputBox.RemoveItem();
      RegenerateInputItems();
    }

    // Render Frame
    BeginDrawing();

    ClearBackground(RAYWHITE);
    if (timer >= 0) {
      DrawText(TextFormat("Score: %04i", score), 10, 10, 40, BLACK);

      DrawText(TextFormat("%02i:%02i",
                          timer / RenderConstants::kTargetFPS /
                              GameConstants::kSecondsPerMinute,
                          timer / RenderConstants::kTargetFPS %
                              GameConstants::kSecondsPerMinute),
               RenderConstants::kScreenWidth - 110, 10, 40, BLACK);

      DrawRectangleRec(leftInput->GetRect(), BLACK);

      DrawRectangleRec(rightInput->GetRect(), BLACK);

      DrawRectangleRec(outputBox.GetRect(), BLUE);

      DrawRectangleRec(optionBox_left.GetRect(), RED);

      DrawRectangleRec(optionBox_center.GetRect(), RED);

      DrawRectangleRec(optionBox_right.GetRect(), RED);

      DrawTexture(
          addIcon, static_cast<int>(RenderConstants::kScreenWidth / 3.65f),
          static_cast<int>(RenderConstants::kScreenHeight / 8.0f), WHITE);

      DrawTexture(
          equalIcon, static_cast<int>(RenderConstants::kScreenWidth / 1.7f),
          static_cast<int>(RenderConstants::kScreenHeight / 8.0f), WHITE);

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
    } else {
      DrawText(TextFormat("Score: %04i", score), 10, 10, 40, BLACK);

      DrawText(TextFormat("%02i:%02i", 0, 0),
               RenderConstants::kScreenWidth - 110, 10, 40, BLACK);
      DrawText("GAME OVER", 190, 200, 20, BLACK);
    }
    EndDrawing();

    timer--;
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

bool AddItem(std::shared_ptr<Item> item) {
  auto it = std::find(items.begin(), items.end(), item);

  if (it == items.end()) {
    items.push_back(item);
    return true;
  }

  return false;
}

void RegenerateInputItems() {
  bool leftHas = !optionBox_left.IsHoldingItem();
  bool centerHas = !optionBox_center.IsHoldingItem();
  bool rightHas = !optionBox_right.IsHoldingItem();

  do {
    if (leftHas) optionBox_left.SpawnItemInBox();
    if (centerHas) optionBox_center.SpawnItemInBox();
    if (rightHas) optionBox_right.SpawnItemInBox();
  } while (!IsValidOptionCombination(optionBox_left.GetHeldItem()->GetType(),
                                     optionBox_center.GetHeldItem()->GetType(),
                                     optionBox_right.GetHeldItem()->GetType()));

  if (leftHas) AddItem(optionBox_left.GetHeldItem());
  if (rightHas) AddItem(optionBox_right.GetHeldItem());
  if (centerHas) AddItem(optionBox_center.GetHeldItem());
}