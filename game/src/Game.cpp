#include "Game.h"

#include <iostream>

int main() {
  std::cout << "starting" << std::endl;
  // Start Rendering Window
  InitWindow(RenderConstants::kScreenWidth, RenderConstants::kScreenHeight,
             "raylib test");

  SetTargetFPS(RenderConstants::kTargetFPS);

  // Load Textures from Item Images
  Texture2D itemTexture = {LoadTextureFromImage(testItem->GetImage())};
  Texture2D starTexture = {LoadTextureFromImage(starItem->GetImage())};

  items.push_back(starItem);
  items.push_back(testItem);

  textures.reserve(items.size());
  textures.push_back(starTexture);

  textures.insert(textures.begin() + testItem->GetID(), itemTexture);
  textures.insert(textures.begin() + starItem->GetID(), starTexture);

  optionBox.SpawnItemInBox();

  items.push_back(optionBox.GetHeldItem());

  Texture2D optionTexture = {
      LoadTextureFromImage(optionBox.GetHeldItem()->GetImage())};

  textures.insert(textures.begin() + optionBox.GetHeldItem()->GetID(),
                  optionTexture);

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

    if (outputTexture.has_value() && outputItem == nullptr) {
      UnloadTexture(outputTexture.value());
      outputTexture = std::nullopt;
    }

    // Render Frame
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawRectangleRec(leftInput->GetRect(), BLACK);

    DrawRectangleRec(rightInput->GetRect(), BLACK);

    DrawRectangleRec(outputBox.GetRect(), BLUE);

    DrawRectangleRec(optionBox.GetRect(), RED);

    DrawText("FOOD GAME", 190, 200, 20, BLACK);

    // Render loaded items.

    for (auto &item : items) {
      DrawTexturePro(textures[item->GetID() + 1], item->GetRect(),
                     {item->GetPosition().x, item->GetPosition().y,
                      item->GetRect().width, item->GetRect().height},
                     item->GetCenter(), 0, WHITE);

      DrawText(item->GetName().c_str(),
               (int)item->GetPosition().x + RenderConstants::kTextOffsetX,
               (int)item->GetPosition().y + RenderConstants::kTextOffsetY, 20,
               BLACK);
    }

    if (outputItem != nullptr) {
      if (!outputTexture.has_value())
        outputTexture = {LoadTextureFromImage(outputItem->GetImage())};

      DrawTexturePro(
          outputTexture.value(), outputItem->GetRect(),
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
  for (int i = 0; i < textures.size(); i++) {
    UnloadTexture(textures[i]);
  }

  CloseWindow();

  return 0;
}