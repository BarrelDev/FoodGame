#include "ParticleSystem.h"

#include <raylib.h>

#include <memory>
#include <random>
#include <vector>

namespace ParticleConstants {
constexpr int kMaxParticles = 10;
constexpr float kTopYOffset = 1.f / (1.7320508076f * 2.f);
constexpr float kBottomYOffset = 1.f / 1.7320508076f;
constexpr float kXOffset = 0.5f;
constexpr float kYAccel = 1.f;
};  // namespace ParticleConstants

Vector2 operator+(Vector2 const &v1, Vector2 const &v2) {
  Vector2 v{v1.x + v2.x, v1.y + v2.y};
  return v;
}

Vector2 operator*(float const &d, Vector2 const &v) {
  return Vector2{v.x * d, v.y * d};
}
Vector2 operator*(Vector2 const &v, float const &d) {
  return Vector2{v.x * d, v.y * d};
}

std::random_device pRd;
std::mt19937 pGen(pRd());
std::uniform_real_distribution<float> xDis(-20.f, 20.f);
std::uniform_real_distribution<float> yDis(-20.f, 50.f);
std::uniform_real_distribution<float> vxDis(-5.f, 5.f);
std::uniform_real_distribution<float> vyDis(-5.f, 5.f);
std::uniform_int_distribution<int> dis(0, 100);

Particle::Particle() {
  velocity = Vector2{vxDis(pGen), vyDis(pGen)};
  acceleration = Vector2{0.f, ParticleConstants::kYAccel};
}

Particle::Particle(Vector2 pos) : position{pos} {
  velocity = Vector2{vxDis(pGen), vyDis(pGen)};
  acceleration = Vector2{0.f, ParticleConstants::kYAccel};
}

void Particle::Draw() const {
  DrawTriangle(
      position + size * Vector2{-ParticleConstants::kXOffset,
                                ParticleConstants::kTopYOffset},
      position + size * Vector2{ParticleConstants::kTopYOffset,
                                ParticleConstants::kXOffset},
      position + size * Vector2{ParticleConstants::kBottomYOffset, 0.f}, RED);
}

void Particle::Update() {
  Vector2 newVel = Vector2{0.98f * (velocity.x + acceleration.x),
                           0.98f * (velocity.y + acceleration.y)};
  velocity = newVel;
  position = position + velocity;
  if (dis(pGen) < 30) size -= 1.f;
}

ParticleSystem::ParticleSystem(Vector2 pos) noexcept {
  system.reserve(ParticleConstants::kMaxParticles);
  for (int i = 0; i < ParticleConstants::kMaxParticles; i++) {
    system.emplace_back(pos + Vector2{xDis(pGen), yDis(pGen)});
  }
}

void ParticleSystem::Draw() const {
  for (const auto &p : system) {
    if (p.position.x < 0 || p.position.x > GetScreenWidth() ||
        p.position.y < 0 || p.position.y > GetScreenHeight()) {
      continue;
    }
    p.Draw();
  }
}

void ParticleSystem::Update() {
  system.erase(std::remove_if(system.begin(), system.end(),
                              [](auto &p) {
                                p.Update();
                                return p.size <= 0.f || p.position.x < 0 ||
                                       p.position.x > GetScreenWidth() ||
                                       p.position.y < 0 ||
                                       p.position.y > GetScreenHeight();
                                ;
                              }),
               system.end());
}