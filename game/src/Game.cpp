#include "Game.h"

void main()
{
	// Start Rendering Window
	InitWindow(RenderConstants::kScreenWidth, RenderConstants::kScreenHeight, "raylib test");

	SetTargetFPS(RenderConstants::kTargetFPS);

	// Load Textures from Item Images
	Texture2D itemTexture = { LoadTextureFromImage(testItem.GetImage()) };
	Texture2D starTexture = { LoadTextureFromImage(starItem.GetImage()) };

	

	// Main Game Loop
	while (!WindowShouldClose()) 
	{
		// Check if mouse button held
		if (IsMouseButtonDown(0))
			isHolding = true;
		else if (IsMouseButtonReleased(0))
			isHolding = false;

		// Move starItem to mouse position if left mouse button held
		if (isHolding) 
		{
			mousePos = GetMousePosition();
			if (mousePos.x <= (float)RenderConstants::kScreenWidth - starItem.GetCenter().x && mousePos.x >= starItem.GetCenter().x)
				starItem.SetX(mousePos.x);
			if (mousePos.y <= (float)RenderConstants::kScreenHeight - starItem.GetCenter().y && mousePos.y >= starItem.GetCenter().y)
				starItem.SetY(mousePos.y);
		}

		// Render Frame
		BeginDrawing();

			ClearBackground(RAYWHITE);

			DrawRectangleRec(leftInput.GetRect(), BLACK);

			DrawRectangleRec(rightInput.GetRect(), BLACK);

			DrawRectangleRec(output, BLUE);

			DrawRectangleRec({ starItem.GetPosition().x - starItem.GetCenter().x, starItem.GetPosition().y - starItem.GetCenter().y, starItem.GetRect().width, starItem.GetRect().height }, BLUE);

			DrawText("I MADE MY FIRST WINDOW IN RAYLIB!!!", 190, 200, 20, BLACK);

			DrawCircleV(ballPosition, 50, MAROON);

			DrawTextureV(itemTexture, testItem.GetPosition(), WHITE);

			DrawTexturePro(starTexture, starItem.GetRect(), { starItem.GetPosition().x, starItem.GetPosition().y, starItem.GetRect().width, starItem.GetRect().height }, starItem.GetCenter(), 0, WHITE);

			DrawText(starItem.GetName().c_str(), (int)starItem.GetPosition().x + RenderConstants::kTextOffsetX, (int)starItem.GetPosition().y + RenderConstants::kTextOffsetY, 20, BLACK);
			
			if(leftInput.IsItemTouching(&starItem) || rightInput.IsItemTouching(&starItem))
                          DrawText("yes", 200, 300, 20, BLACK);
                        else
                          DrawText("no", 200, 300, 20, BLACK);


		EndDrawing();
	}

	// Unload Textures
	UnloadTexture(itemTexture);
	UnloadTexture(starTexture);

	CloseWindow();

	return;
}