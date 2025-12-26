#pragma once

#include "Constants.h"
#include "raylib.h"
#include <cmath>
#include <algorithm>

constexpr float kMinPixels = std::min(RenderConstants::kScreenWidth, RenderConstants::kScreenHeight) / 20.f;

// Helpers to convert from "design" coordinates (RenderConstants::kScreenWidth/Height)
// to actual runtime coordinates (GetScreenWidth/GetScreenHeight).

constexpr inline float GetScaleX() {
    return static_cast<float>(RenderConstants::kScreenWidth) / static_cast<float>(RenderConstants::kInternalScreenWidth);
}
constexpr inline float GetScaleY() {
  return static_cast<float>(RenderConstants::kScreenHeight) /
         static_cast<float>(RenderConstants::kInternalScreenHeight);
}
// Uniform scale keeps aspect ratio (recommended for sprites/UI)
inline float GetUniformScale() {
    return std::min(GetScaleX(), GetScaleY());
}

inline float ScaleX(float x) { return std::max(kMinPixels, x * GetScaleX()); }
inline float ScaleY(float y) { return std::max(kMinPixels, y * GetScaleY()); }
inline float ScaleUniform(float v) { return std::max(kMinPixels, v * GetUniformScale()); }
inline Vector2 Scale(Vector2 v) {
  return Vector2{ScaleX(v.x), ScaleY(v.y)}; }
inline Vector2 ScaleVecUniform(Vector2 v) {
  float s = GetUniformScale();
  return Vector2{v.x * s, v.y * s};
}

// UNscale helpers (screen -> design coordinates)
inline float UnscaleX(float x) { return std::max(kMinPixels, x / GetScaleX()); }
inline float UnscaleY(float y) { return std::max(kMinPixels, y / GetScaleY()); }
inline Vector2 Unscale(Vector2 v) {
  return Vector2{UnscaleX(v.x), UnscaleY(v.y)};
}
inline float UnscaleUniform(float v) { return std::max(kMinPixels, v / GetUniformScale()); }

// integer helpers for API calls that need ints
inline int Sxi(float x) { return static_cast<int>(std::round(ScaleX(x))); }
inline int Syi(float y) { return static_cast<int>(std::round(ScaleY(y))); }
inline int Si(float v)  { return static_cast<int>(std::round(ScaleUniform(v))); }

// optional unscale to integer design coords
inline int USi(float v) { return static_cast<int>(std::round(UnscaleUniform(v))); }

// Scale a raylib Rectangle (position + size). If you want to preserve aspect ratio
// use ScaleUniform for width/height instead of ScaleX/ScaleY.
inline Rectangle ScaleRect(const Rectangle& r, bool uniform = true) {
    if (uniform) {
        float s = GetUniformScale();
        return Rectangle{ ScaleX(r.x), ScaleY(r.y), std::max(kMinPixels, r.width * s), std::max(kMinPixels, r.height * s) };
    } else {
        return Rectangle{ ScaleX(r.x), ScaleY(r.y), ScaleX(r.width), ScaleY(r.height) };
    }
}

inline Rectangle UnscaleRect(const Rectangle& r, bool uniform = true) {
  if (uniform) {
    float s = GetUniformScale();
    return Rectangle{UnscaleX(r.x), UnscaleY(r.y), std::max(kMinPixels, r.width / s), std::max(kMinPixels, r.height / s)};
  } else {
    return Rectangle{UnscaleX(r.x), UnscaleY(r.y), UnscaleX(r.width),
                     UnscaleY(r.height)};
  }
}