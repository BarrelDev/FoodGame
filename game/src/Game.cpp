#include "Game.h"

void main()
{
	// Start Rendering Window
	InitWindow(RenderConstants::kScreenWidth, RenderConstants::kScreenHeight, "raylib test");

	SetTargetFPS(RenderConstants::kTargetFPS);

	// Testing ECS
	EntityID starECS = scene.NewEntity();
	PositionComponent* testPos = scene.Assign<PositionComponent>(starECS);
	TextureComponent* testTxt = scene.Assign<TextureComponent>(starECS);

	SetPosition(*testPos, (float)RenderConstants::kScreenWidth / 3.0f, 100.0f);
	SetTexture(*testTxt, "resources/textures/star.png");

	// Load Textures from Item Images
	Texture2D itemTexture = { LoadTextureFromImage(testItem.GetImage()) };
	Texture2D starTexture = { LoadTextureFromImage(starItem.GetImage()) };
	Texture2D ecsTexture = { LoadTextureFromImage(testTxt->img) };

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

			DrawTextureV(ecsTexture, testPos->pos, WHITE);

			DrawTexturePro(starTexture, starRect, { starItem.GetPosition().x, starItem.GetPosition().y, starRect.width, starRect.height }, starCenter, 0, WHITE);

			DrawText(starItem.GetName().c_str(), (int) starItem.GetPosition().x + RenderConstants::kTextOffsetX, (int) starItem.GetPosition().y + RenderConstants::kTextOffsetY, 20, BLACK);

		EndDrawing();
	}

	// Unload Textures
	UnloadTexture(itemTexture);
	UnloadTexture(starTexture);

	CloseWindow();

	return;
}

void SetPosition(PositionComponent& pos, float x, float y) 
{
	pos.pos.x = x;
	pos.pos.x = y;
}

void SetTexture(TextureComponent& txt, const char *filePath) 
{
	txt.filePath = filePath;
	txt.img = LoadImage(txt.filePath);
}
