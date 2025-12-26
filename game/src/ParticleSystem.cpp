#include "ParticleSystem.h"

#include <raylib.h>

#include <memory>
#include <random>
#include <vector>
#include "Scale.h"

namespace ParticleConstants {
constexpr int kMaxParticles = 10;
constexpr float kTopYOffset = 1.f / (1.7320508076f * 2.f);
constexpr float kBottomYOffset = 1.f / 1.7320508076f;
constexpr float kXOffset = 0.5f;
constexpr float kYAccel = 1.f;

constexpr float kHeight = 1.7320508076f / 2.0f;
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
std::uniform_real_distribution<float> xDis(-10.f, 10.f);
std::uniform_real_distribution<float> yDis(-10.f, 25.f);
std::uniform_real_distribution<float> vxDis(-10.f, 10.f);
std::uniform_real_distribution<float> vyDis(-10.f, 5.f);
std::uniform_int_distribution<int> dis(0, 100);

Particle::Particle() {
  velocity = Vector2{vxDis(pGen), vyDis(pGen)};
  acceleration = Vector2{0.f, ParticleConstants::kYAccel};
}

Particle::Particle(Vector2 pos) : position{pos} {
  velocity = Vector2{vxDis(pGen), vyDis(pGen)};
  acceleration = Vector2{0.f, ParticleConstants::kYAccel};
}

Particle::Particle(Vector2 pos, float s, Color c)
    : position{pos}, size{s}, color{c} {
  velocity = Vector2{vxDis(pGen), vyDis(pGen)};
  acceleration = Vector2{0.f, ParticleConstants::kYAccel};
}

void Particle::Draw() const {
  float height = ParticleConstants::kHeight * size;

  Vector2 A = {0.f, height / 3.0f * 2.0f};
  Vector2 B = {size / 2.0f, -height / 3.f};
  Vector2 C = {-size / 2.0f, -height / 3.f};

  A = ScaleVecUniform(A + position);
  B = ScaleVecUniform(B + position);
  C = ScaleVecUniform(C + position);

  DrawTriangle(A, B, C, color);
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

ParticleSystem::ParticleSystem(Vector2 pos, float size, Color color) noexcept {
  system.reserve(ParticleConstants::kMaxParticles);
  for (int i = 0; i < ParticleConstants::kMaxParticles; i++) {
    system.emplace_back(pos + Vector2{xDis(pGen), yDis(pGen)}, size, color);
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