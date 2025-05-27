#pragma once

#include <raylib.h>

#include <vector>

class Particle {
 public:
  Vector2 acceleration;
  Vector2 position{0.f};
  Vector2 velocity{0.f};
  float size{25.f};

  Particle();

  Particle(Vector2 pos);

  Particle(Vector2 pos, float s);

  void Draw() const;

  void Update();
};

class ParticleSystem {
 public:
  std::vector<Particle> system;

  ParticleSystem(Vector2 pos) noexcept;

  void Draw() const;

  void Update();
};