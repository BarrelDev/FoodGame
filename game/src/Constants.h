#pragma once

namespace RenderConstants {
constexpr int kScreenWidth = 800;
constexpr int kScreenHeight = 450;

constexpr int kTargetFPS = 30;

constexpr int kTextOffsetY = 50;
constexpr int kTextOffsetX = -20;

constexpr float kMaxShakeAngle = 30.f;  // in degrees
constexpr float kMaxShakeOffset = 50.f;
}  // namespace RenderConstants

namespace GameConstants {
constexpr int kTotalFrames = 3000;
constexpr int kSecondsPerMinute = 60;
constexpr int kMultiplierTime = RenderConstants::kTargetFPS * 5;
constexpr int kMinMultiplierTime = 15;
constexpr float kMultiplierGrowthRate = 0.5f;

constexpr int testMinute =
    kTotalFrames / RenderConstants::kTargetFPS / kSecondsPerMinute;
constexpr int testSeconds =
    kTotalFrames / RenderConstants::kTargetFPS % kSecondsPerMinute;
};  // namespace GameConstants

namespace BoxConstants {
constexpr int kBoxWidth = 150;
constexpr int kBoxHeight = 100;
}  // namespace BoxConstants