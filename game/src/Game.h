#pragma once

#include <memory>
#include <vector>

#include "Constants.h"
#include "InputBox.h"
#include "Item.h"
#include "OptionBox.h"
#include "OutputBox.h"
#include "ParticleSystem.h"
#include "Scale.h"
#include "raylib.h"

// Store ball shape position
extern Vector2 ballPosition{RenderConstants::kScreenWidth / 2.0f,
                     RenderConstants::kScreenHeight / 4.0f};

// Store items and their respective textures dynamically for easy access.
std::vector<std::shared_ptr<Item>> items;

// Collision for items.
extern std::shared_ptr<InputBox> leftInput{
        std::make_shared<InputBox>((float)RenderConstants::kInternalScreenWidth / 12.0f,
                                   (float)RenderConstants::kInternalScreenHeight / 8.0f)};

extern std::shared_ptr<InputBox> rightInput{
        std::make_shared<InputBox>((float)RenderConstants::kInternalScreenWidth / 2.5f,
                                   (float)RenderConstants::kInternalScreenHeight / 8.0f)};

extern OutputBox outputBox{(float)RenderConstants::kInternalScreenWidth / 1.4f,
                           (float)RenderConstants::kInternalScreenHeight / 8.0f,
                           leftInput,
                        rightInput};

extern OptionBox optionBox_left{
    (float)RenderConstants::kInternalScreenWidth / 12.0f,
    (float)RenderConstants::kInternalScreenHeight / 1.5f};

extern OptionBox optionBox_center{
    (float)RenderConstants::kInternalScreenWidth / 2.5f,
    (float)RenderConstants::kInternalScreenHeight / 1.5f};

extern OptionBox optionBox_right{
    (float)RenderConstants::kInternalScreenWidth / 1.4f,
    (float)RenderConstants::kInternalScreenHeight / 1.5f};

// Temporary Rects for storing item spawn boxes.
Rectangle optionOne;
Rectangle optionTwo;
Rectangle optionThree;

extern Rectangle replayButton{RenderConstants::kReplayX, RenderConstants::kReplayY, 145, 30};

Camera2D camera;
Camera2D baseCamera;

// Store mouse position
Vector2 mousePos;

// Particles
std::vector<ParticleSystem> pSystems{};

bool isHolding;
bool holdingOutput;

std::shared_ptr<Item> heldItem;

std::shared_ptr<Item> outputItem;

bool isLeftClicking;
bool isRightClicking;

static int score = 0;
static int multipliedScores = 0;

static float trauma = 0.f;

static int coyoteTime = GameConstants::kCoyoteTime;
static bool coyoteActive = false;

static int timeSinceLastScore = GameConstants::kTotalFrames;

static int prevScoreTime = GameConstants::kTotalFrames;

static int timer = GameConstants::kTotalFrames;

static float multiplier = 1.0f;

static int multiplierTime = GameConstants::kMultiplierTime;

int main();

bool IsValidOptionCombination(ItemType one, ItemType two, ItemType three);

/**
    Helper method for quickly removing items from the draw calls.
    @return Whether the destroy was successful or not.
*/
bool DestroyItem(std::shared_ptr<Item> item);

/**
    Helper method for quickly adding items from the draw calls without repeating
   items.
    @return Whether the add was successful or not.
*/
bool AddItem(std::shared_ptr<Item> item);

/*
    Regenerates items into input boxes.
*/
void RegenerateInputItems();

float GetMultiplier();

void ResetCameraPosition();

void RestartGame();