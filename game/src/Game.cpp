#include "Game.h"

#include <iostream>

int main() {
  std::cout << "starting" << std::endl;
  // Start Rendering Window
  InitWindow(RenderConstants::kScreenWidth, RenderConstants::kScreenHeight,
             "raylib test");

  SetTargetFPS(RenderConstants::kTargetFPS);

  optionBox_left.SpawnItemInBox();
  optionBox_center.SpawnItemInBox();
  optionBox_right.SpawnItemInBox();

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

    // Render Frame
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawRectangleRec(leftInput->GetRect(), BLACK);

    DrawRectangleRec(rightInput->GetRect(), BLACK);

    DrawRectangleRec(outputBox.GetRect(), BLUE);

    DrawRectangleRec(optionBox_left.GetRect(), RED);

    DrawRectangleRec(optionBox_center.GetRect(), RED);

    DrawRectangleRec(optionBox_right.GetRect(), RED);

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

    if (outputItem != nullptr) {
      TextureManager::UpdateItemTextures(outputItem);

      DrawTexturePro(
          TextureManager::GetTextureFromItemType(outputItem->GetType()),
          outputItem->GetRect(),
          {outputItem->GetPosition().x, outputItem->GetPosition().y,
           outputItem->GetRect().width, outputItem->GetRect().height},
          outputItem->GetCenter(), 0, WHITE);

      DrawText(
          outputItem->GetName().c_str(),
          (int)outputItem->GetPosition().x + RenderConstants::kTextOffsetX - 10,
          (int)outputItem->GetPosition().y + RenderConstants::kTextOffsetY, 20,
          BLACK);
    }

    EndDrawing();
  }

  // Unload Textures
  TextureManager::UnloadItemTextures();

  CloseWindow();

  return 0;
}