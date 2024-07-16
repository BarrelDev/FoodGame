#include "Game.h"

#include <iostream>

void main()
{
  std::cout << "starting" << std::endl;
	// Start Rendering Window
	InitWindow(RenderConstants::kScreenWidth, RenderConstants::kScreenHeight, "raylib test");

	SetTargetFPS(RenderConstants::kTargetFPS);

	// Load Textures from Item Images
	Texture2D itemTexture = { LoadTextureFromImage(testItem.GetImage()) };
	Texture2D starTexture = { LoadTextureFromImage(starItem->GetImage()) };

	items.push_back(starItem);
    items.push_back(std::make_shared<Item>(testItem));

	textures.reserve(items.size());
	textures.push_back(starTexture);


	textures.insert(textures.begin() + testItem.GetID(), itemTexture);
    textures.insert(textures.begin() + starItem->GetID(), starTexture);

	

	// Main Game Loop
	while (!WindowShouldClose()) 
	{
      mousePos = GetMousePosition();
		// Check if mouse button held
		if (IsMouseButtonDown(0))
			isLeftClicking = true;
		else if (IsMouseButtonReleased(0))
			isLeftClicking = false;

		if (isLeftClicking) 
		{
                  Vector2 itemPos;
                  Vector2 itemEdge;
                  for (int i = 0; i < items.size(); ++i) 
				  {
                    std::shared_ptr<Item> item = items[i];

                    itemPos = Vector2{item->GetPosition().x - item->GetWidth() / 2.0f,
                               item->GetPosition().y - item->GetHeight() / 2.0f};
                    itemEdge = Vector2{item->GetPosition().x + item->GetWidth() / 2.0f,
                        item->GetPosition().y + item->GetHeight() / 2.0f};
                    if (mousePos.x >= itemPos.x && mousePos.x <= itemEdge.x &&
                        mousePos.y >= itemPos.y && mousePos.y <= itemEdge.y) 
					{
                      isHolding = true;
                      heldItem = item;
                    }
				  }
                } else {
                  isHolding = false;
		}

		// Move heldItem to mouse position if left mouse button held
		if (isHolding) 
		{
			if (mousePos.x <= (float)RenderConstants::kScreenWidth - heldItem->GetCenter().x && mousePos.x >= heldItem->GetCenter().x)
				heldItem->SetX(mousePos.x);
			if (mousePos.y <= (float)RenderConstants::kScreenHeight - heldItem->GetCenter().y && mousePos.y >= heldItem->GetCenter().y)
                heldItem->SetY(mousePos.y);
        } else if(heldItem != nullptr) {
                  if (leftInput.IsItemTouching(heldItem))
                    leftInput.SnapItemInBox(heldItem);
                  if (rightInput.IsItemTouching(heldItem))
                    rightInput.SnapItemInBox(heldItem);
		}

		// Render Frame
		BeginDrawing();

			ClearBackground(RAYWHITE);

			DrawRectangleRec(leftInput.GetRect(), BLACK);

			DrawRectangleRec(rightInput.GetRect(), BLACK);

			DrawRectangleRec(output, BLUE);

			DrawText("I MADE MY FIRST WINDOW IN RAYLIB!!!", 190, 200, 20, BLACK);

			for (auto &item : items) 
			{
                          DrawTexturePro(textures[item->GetID() + 1], item->GetRect(),
                                         {item->GetPosition().x,
                                          item->GetPosition().y,
                                          item->GetRect().width, item->GetRect().height},
                              item->GetCenter(), 0, WHITE);

                          DrawText(item->GetName().c_str(),
                                   (int)item->GetPosition().x +
                                       RenderConstants::kTextOffsetX,
                                   (int)item->GetPosition().y +
                                       RenderConstants::kTextOffsetY,
                                   20, BLACK);
            }
			
			if(leftInput.IsItemTouching(heldItem) || rightInput.IsItemTouching(heldItem))
                          DrawText("yes", 200, 300, 20, BLACK);
                        else
                          DrawText("no", 200, 300, 20, BLACK);
		EndDrawing();
	}

	// Unload Textures
    for (int i = 0; i < textures.size(); i++) 
	{
          UnloadTexture(textures[i]);
	}

	//UnloadTexture(itemTexture);
	//UnloadTexture(starTexture);

	CloseWindow();

	return;
}