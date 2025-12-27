#pragma once

namespace RenderConstants {
constexpr int kScreenWidth = 2560;
constexpr int kScreenHeight = 1440;
constexpr int kInternalScreenWidth = 1280;
constexpr int kInternalScreenHeight = 720;

constexpr int kTargetFPS = 30;

constexpr int kTextOffsetY = 80;
constexpr int kTextOffsetX = -32;

constexpr float kMaxShakeAngle = 30.f;  // in degrees
constexpr float kMaxShakeOffset = 50.f;

constexpr int kMinBonusTextSize = 15;
}  // namespace RenderConstants

namespace GameConstants {
constexpr int kTotalFrames = 3000;
constexpr int kSecondsPerMinute = 60;
constexpr int kMultiplierTime = RenderConstants::kTargetFPS * 5;
constexpr int kMinMultiplierTime = 15;
constexpr float kMultiplierGrowthRate = 0.5f;
constexpr float kHitboxForgiveness = 20.f;
constexpr int kCoyoteTime = 1;

constexpr int testMinute =
    kTotalFrames / RenderConstants::kTargetFPS / kSecondsPerMinute;
constexpr int testSeconds =
    kTotalFrames / RenderConstants::kTargetFPS % kSecondsPerMinute;
};  // namespace GameConstants

namespace BoxConstants {
constexpr int kBoxWidth = 240;
constexpr int kBoxHeight = 160;
}  // namespace BoxConstants