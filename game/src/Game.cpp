#include "Game.h"

#include <ParticleSystem.h>
#include <raylib.h>

#include <__msvc_ostream.hpp>
#include <cmath>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include "Constants.h"
#include "Item.h"
#include "ItemFactory.h"
#include "OutputBox.h"
#include "TextureManager.h"
#include "Scale.h"

int main() {
  std::cout << "starting" << std::endl;
  // Start Rendering Window
  InitWindow(RenderConstants::kScreenWidth, RenderConstants::kScreenHeight,
             "raylib test");

  SetTargetFPS(RenderConstants::kTargetFPS);

  baseCamera.offset = Vector2{RenderConstants::kScreenWidth / 2.0f,
                              RenderConstants::kScreenHeight / 2.0f};
  baseCamera.target = Vector2{RenderConstants::kScreenWidth / 2.0f,
                              RenderConstants::kScreenHeight / 2.0f};
  baseCamera.rotation = 0.f;
  baseCamera.zoom = 1.0f;

  pSystems.reserve(10);

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

  ResetCameraPosition();

  // Initialize Random Engine & Distribution
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis(-1.0, 1.0);

  // Main Game Loop
  while (!WindowShouldClose()) {
    mousePos = Unscale(GetMousePosition());
    // Check if mouse button held
    if (IsMouseButtonDown(0))
      isLeftClicking = true;
    else if (IsMouseButtonReleased(0))
      isLeftClicking = false;

    if (IsKeyPressed(KEY_L)) timer = -1;

    // Check if item is in range of being held by mouse cursor.
    if (isLeftClicking) {
      Vector2 itemPos;
      Vector2 itemEdge;
      for (int i = 0; i < items.size(); ++i) {
        std::shared_ptr<Item> item = items[i];

        itemPos = Vector2{item->GetPosition().x - item->GetWidth() / 2.0f -
                              Si(GameConstants::kHitboxForgiveness) * 2.0f,
                          item->GetPosition().y - item->GetHeight() / 2.0f -
                              Si(GameConstants::kHitboxForgiveness)};
        itemEdge = Vector2{item->GetPosition().x + item->GetWidth() / 2.0f +
                               Si(GameConstants::kHitboxForgiveness) * 2.0f,
                           item->GetPosition().y + item->GetHeight() / 2.0f +
                               Si(GameConstants::kHitboxForgiveness)};
        if (mousePos.x >= itemPos.x && mousePos.x <= itemEdge.x &&
            mousePos.y >= itemPos.y && mousePos.y <= itemEdge.y) {
          isHolding = true;
          heldItem = item;
        }
      }
    } else {
      if (isHolding) coyoteActive = true;
    }

    if (coyoteActive) {
      if (coyoteTime > 0)
        coyoteTime--;
      else {
        coyoteTime = GameConstants::kCoyoteTime;
        coyoteActive = false;
        isHolding = false;
      }
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
      bool isInput = ItemFactory::IsInputType(heldItem->GetType());
      if (leftInput->IsItemTouching(heldItem) && !leftInput->IsHoldingItem() &&
          isInput) {
        leftInput->SnapItemInBox(heldItem);
        heldItem = nullptr;
      } else if (leftInput->GetHeldItem() == heldItem) {
        leftInput->RemoveItem();
      }
      if (rightInput->IsItemTouching(heldItem) &&
          !rightInput->IsHoldingItem() && isInput) {
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
      AddItem(outputItem);

      // Delete the input items once the output has been picked up.
      if (heldItem == outputItem && leftInput->IsHoldingItem() &&
          rightInput->IsHoldingItem()) {
        if (optionBox_left.GetHeldItem() == leftInput->GetHeldItem()) {
          optionBox_left.RemoveItem();
        }
        if (optionBox_center.GetHeldItem() == leftInput->GetHeldItem()) {
          optionBox_center.RemoveItem();
        }
        if (optionBox_right.GetHeldItem() == leftInput->GetHeldItem()) {
          optionBox_right.RemoveItem();
        }
        DestroyItem(leftInput->GetHeldItem());
        leftInput->RemoveItem();
        if (optionBox_left.GetHeldItem() == rightInput->GetHeldItem()) {
          optionBox_left.RemoveItem();
        }
        if (optionBox_center.GetHeldItem() == rightInput->GetHeldItem()) {
          optionBox_center.RemoveItem();
        }
        if (optionBox_right.GetHeldItem() == rightInput->GetHeldItem()) {
          optionBox_right.RemoveItem();
        }
        DestroyItem(rightInput->GetHeldItem());
        rightInput->RemoveItem();

        holdingOutput = true;

        score += static_cast<int>(GetMultiplier());
        timeSinceLastScore = prevScoreTime - timer;
        prevScoreTime = timer;
        multipliedScores++;

        /*std::cout << "Score: " << score << " Time Since: " <<
           timeSinceLastScore
                  << " Prev Time: " << prevScoreTime
                  << " Multiplier: " << multiplier << std::endl;*/
      }
    }

    if (holdingOutput && !isHolding) {
      holdingOutput = false;
      pSystems.emplace_back(heldItem->GetPosition());
      if (ItemFactory::IsOutputType(heldItem->GetType())) DestroyItem(heldItem);
      DestroyItem(outputBox.GetHeldItem());
      outputBox.RemoveItem();
      RegenerateInputItems();

      heldItem = nullptr;
    }

    // Apply Camera Shake
    float shakeAngle =
        RenderConstants::kMaxShakeAngle * std::powf(trauma, 2.f) * dis(gen);
    float shakeX =
        RenderConstants::kMaxShakeOffset * std::powf(trauma, 2.f) * dis(gen);
    float shakeY =
        RenderConstants::kMaxShakeOffset * std::powf(trauma, 2.f) * dis(gen);

    camera.rotation = baseCamera.rotation + shakeAngle;
    camera.target =
        Vector2{baseCamera.target.x + shakeX, baseCamera.target.y + shakeY};

    // Render Frame
    BeginDrawing();

    ClearBackground(RAYWHITE);
    BeginMode2D(camera);
    if (timer >= 0) {
      int scoreFont = Si(40);
      int bonusFont = Si(20 * multiplier);
      DrawText(TextFormat("Score: %04i", score), ScaleX(10), ScaleX(10),
               scoreFont,
               BLACK);
      if (multiplier > 1.1) {
        DrawText(TextFormat("Bonus: x%4.2f", multiplier), ScaleX(10), ScaleY(220),
                 bonusFont, BLACK);
      }

      DrawText(TextFormat("%02i:%02i",
                          timer / RenderConstants::kTargetFPS /
                              GameConstants::kSecondsPerMinute,
                          timer / RenderConstants::kTargetFPS %
                              GameConstants::kSecondsPerMinute),
               ScaleX(RenderConstants::kInternalScreenWidth - 110), ScaleY(10), Si(40), BLACK);

      DrawRectangleRec(ScaleRect(leftInput->GetRect()), BLACK);

      DrawRectangleRec(ScaleRect(rightInput->GetRect()), BLACK);

      DrawRectangleRec(ScaleRect(outputBox.GetRect()), BLUE);

      DrawRectangleRec(ScaleRect(optionBox_left.GetRect()), RED);

      DrawRectangleRec(ScaleRect(optionBox_center.GetRect()), RED);

      DrawRectangleRec(ScaleRect(optionBox_right.GetRect()), RED);

      /*DrawTexture(
          addIcon, static_cast<int>(RenderConstants::kScreenWidth / 3.65f),
          static_cast<int>(RenderConstants::kScreenHeight / 8.0f), WHITE);

      DrawTexture(
          equalIcon, static_cast<int>(RenderConstants::kScreenWidth / 1.7f),
          static_cast<int>(RenderConstants::kScreenHeight / 8.0f), WHITE);*/

      // design-space positions (same as before)
      const float addX = RenderConstants::kInternalScreenWidth / 3.65f;
      const float addY = RenderConstants::kInternalScreenHeight / 7.4f;
      const float eqX = RenderConstants::kInternalScreenWidth / 1.7f;
      const float eqY = RenderConstants::kInternalScreenHeight / 7.4f;

      // desired icon size in design coordinates (adjust if you want
      // larger/smaller)
      const float iconDesignSize = 64.f;

      // Source rectangles use the full texture in texture pixels
      Rectangle srcAdd = {0.f, 0.f, (float)addIcon.width,
                          (float)addIcon.height};
      Rectangle srcEqual = {0.f, 0.f, (float)equalIcon.width,
                            (float)equalIcon.height};

      // Destination rectangles are expressed in design coords and then scaled
      // to screen coords
      Rectangle destAddDesign = {addX + iconDesignSize * 0.5f,
                                 addY + iconDesignSize * 0.5f, iconDesignSize,
                                 iconDesignSize};
      Rectangle destEqualDesign = {eqX + iconDesignSize * 0.5f,
                                   eqY + iconDesignSize * 0.5f, iconDesignSize,
                                   iconDesignSize};

      Rectangle destAddScaled = ScaleRect(destAddDesign);
      Rectangle destEqualScaled = ScaleRect(destEqualDesign);

      // Rotation origin is in destination (screen) pixels: use center of the
      // scaled rect
      Vector2 originAdd = {destAddScaled.width * 0.5f,
                           destAddScaled.height * 0.5f};
      Vector2 originEqual = {destEqualScaled.width * 0.5f,
                             destEqualScaled.height * 0.5f};

      DrawTexturePro(addIcon, srcAdd, destAddScaled, originAdd, 0.f, WHITE);
      DrawTexturePro(equalIcon, srcEqual, destEqualScaled, originEqual, 0.f,
                     WHITE);

      DrawText("FOOD GAME", ScaleX(190), ScaleY(200), Si(20), BLACK);

      // Render loaded items.
      // TODO: Need to rescale raw textures for 720p rendering
      // TODO: OR create a texture scaling system, all textures are loaded for 800 x 450 p resolution.
      for (auto item : items) {
        Rectangle dest =
            Rectangle{item->GetPosition().x, item->GetPosition().y,
                      item->GetRect().width * 1.6f, item->GetRect().height * 1.6f};
        Vector2 centerScale = {ScaleX(item->GetCenter().x * 1.6f), ScaleY(item->GetCenter().y*1.6f)};
        DrawTexturePro(TextureManager::GetTextureFromItemType(item->GetType()),
                       item->GetRect(),
                       ScaleRect(dest),
                       centerScale, 0, WHITE);

        int fontSize = Si(20);
        DrawText(item->GetName().c_str(),
                 ScaleX((int)item->GetPosition().x + RenderConstants::kTextOffsetX),
            ScaleY((int)item->GetPosition().y + RenderConstants::kTextOffsetY),
            fontSize,
                 BLACK);
      }

      // Draw Particles

      pSystems.erase(std::remove_if(pSystems.begin(), pSystems.end(),
                                    [](ParticleSystem& sys) {
                                      sys.Update();
                                      sys.Draw();
                                      return sys.system.size() <= 0;
                                    }),
                     pSystems.end());
    } else {
      int scoreFont = Si(40);
      DrawText(TextFormat("Score: %04i", score), ScaleX(10), ScaleY(10),
               scoreFont,
               BLACK);

      DrawText(TextFormat("%02i:%02i", 0, 0),
               RenderConstants::kScreenWidth - 110, ScaleY(10), scoreFont,
               BLACK);

      int overFont = Si(50);
      int replayFont = Si(25);
      DrawText("GAME OVER", ScaleX(250), ScaleY(175), overFont, BLACK);
      if (CheckCollisionPointRec(mousePos, replayButton))
        DrawText("REPLAY", ScaleX(350), ScaleY(220), replayFont, GRAY);
      else
        DrawText("REPLAY", ScaleX(350), ScaleY(220), replayFont, BLACK);
    }
    EndDrawing();

    // End of Frame countdown

    timer--;
    trauma -= 0.01f;
    trauma = std::max(0.f, trauma);

    if (CheckCollisionPointRec(mousePos, replayButton) && timer < 0) {
      if (isLeftClicking) {
        RestartGame();
      }
    }
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

  if (leftHas) {
    AddItem(optionBox_left.GetHeldItem());
    pSystems.emplace_back(optionBox_left.GetHeldItem()->GetPosition(), 15.f,
                          YELLOW);
  }
  if (rightHas) {
    AddItem(optionBox_right.GetHeldItem());
    pSystems.emplace_back(optionBox_right.GetHeldItem()->GetPosition(), 15.f,
                          YELLOW);
  }
  if (centerHas) {
    AddItem(optionBox_center.GetHeldItem());
    pSystems.emplace_back(optionBox_center.GetHeldItem()->GetPosition(), 15.f,
                          YELLOW);
  }
}

float GetMultiplier() {
  if (timeSinceLastScore < multiplierTime) {
    multiplier += GameConstants::kMultiplierGrowthRate;
    trauma += 0.2f;
    trauma = std::min(trauma, 1.0f);
    if (multiplierTime > GameConstants::kMinMultiplierTime)
      multiplierTime = GameConstants::kMultiplierTime / multipliedScores +
                       GameConstants::kMinMultiplierTime;
  } else {
    multiplier = 1.0f;
    multiplierTime = GameConstants::kMultiplierTime;
    multipliedScores = 0;
  }
  return multiplier;
}

void ResetCameraPosition() {
  camera.offset = baseCamera.offset;
  camera.target = baseCamera.target;
  camera.rotation = baseCamera.rotation;
  camera.zoom = baseCamera.zoom;
}

void RestartGame() {
  optionBox_center.RemoveItem();
  optionBox_left.RemoveItem();
  optionBox_right.RemoveItem();

  leftInput->RemoveItem();
  rightInput->RemoveItem();

  items.clear();

  do {
    optionBox_left.SpawnItemInBox();
    optionBox_center.SpawnItemInBox();
    optionBox_right.SpawnItemInBox();
  } while (!IsValidOptionCombination(optionBox_left.GetHeldItem()->GetType(),
                                     optionBox_center.GetHeldItem()->GetType(),
                                     optionBox_right.GetHeldItem()->GetType()));

  AddItem(optionBox_left.GetHeldItem());
  AddItem(optionBox_center.GetHeldItem());
  AddItem(optionBox_right.GetHeldItem());

  score = 0;
  multipliedScores = 0;

  trauma = 0.f;

  coyoteTime = GameConstants::kCoyoteTime;
  coyoteActive = false;

  timeSinceLastScore = GameConstants::kTotalFrames;

  prevScoreTime = GameConstants::kTotalFrames;

  timer = GameConstants::kTotalFrames;

  multiplier = 1.0f;

  multiplierTime = GameConstants::kMultiplierTime;

  ResetCameraPosition();
}