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
			if (mousePos.x <= (float)RenderConstants::kScreenWidth - starCenter.x && mousePos.x >= starCenter.x)
				starItem.SetX(mousePos.x);
			if (mousePos.y <= (float)RenderConstants::kScreenHeight - starCenter.y && mousePos.y >= starCenter.y)
				starItem.SetY(mousePos.y);
		}

		// Render Frame
		BeginDrawing();

			ClearBackground(RAYWHITE);

			DrawText("I MADE MY FIRST WINDOW IN RAYLIB!!!", 190, 200, 20, BLACK);

			DrawCircleV(ballPosition, 50, MAROON);

			DrawTextureV(itemTexture, testItem.GetPosition(), WHITE);

			DrawTexturePro(starTexture, starRect, { starItem.GetPosition().x, starItem.GetPosition().y, starRect.width, starRect.height }, starCenter, 0, WHITE);

			DrawText(starItem.GetName().c_str(), starItem.GetPosition().x + RenderConstants::kTextOffsetX, starItem.GetPosition().y + RenderConstants::kTextOffsetY, 20, BLACK);

		EndDrawing();
	}

	// Unload Textures
	UnloadTexture(itemTexture);
	UnloadTexture(starTexture);

	CloseWindow();

	return;
}