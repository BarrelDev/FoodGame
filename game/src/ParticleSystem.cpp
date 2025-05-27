#include "ParticleSystem.h"

#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

#include <__msvc_ostream.hpp>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

namespace ParticleConstants {
constexpr int kMaxParticles = 10;
constexpr float kTopYOffset = 1.f / (1.7320508076f * 2.f);
constexpr float kBottomYOffset = 1.f / 1.7320508076f;
constexpr float kXOffset = 0.5f;
constexpr float kYAccel = 1.f;

constexpr float kHeight = 1.7320508076f / 2.0f;
constexpr float triangleVerts[] = {
    0.0f,  -1.7320508076f / 3.0f,  // Top vertex
    -0.5f, 1.7320508076f / 6.0f,   // Bottom left
    0.5f,  1.7320508076f / 6.0f    // Bottom right
};
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
// Image particleData = GenImageColor(ParticleConstants::kMaxParticles, 1,
//                                    BLANK);  // use pixels to store pos, size
// Texture2D particleTex = LoadTextureFromImage(particleData);

// Create triangle mesh (once)
unsigned int triangleVBO;

// Create instance buffer
unsigned int instanceVBO;

Particle::Particle() {
  velocity = Vector2{vxDis(pGen), vyDis(pGen)};
  acceleration = Vector2{0.f, ParticleConstants::kYAccel};
}

Particle::Particle(Vector2 pos) : position{pos} {
  velocity = Vector2{vxDis(pGen), vyDis(pGen)};
  acceleration = Vector2{0.f, ParticleConstants::kYAccel};
}

Particle::Particle(Vector2 pos, float s) : position{pos}, size{s} {
  velocity = Vector2{vxDis(pGen), vyDis(pGen)};
  acceleration = Vector2{0.f, ParticleConstants::kYAccel};
}

void Particle::Draw() const {
  float height = ParticleConstants::kHeight * size;

  Vector2 A = {0.f, height / 3.0f * 2.0f};
  Vector2 B = {size / 2.0f, -height / 3.f};
  Vector2 C = {-size / 2.0f, -height / 3.f};

  A = A + position;
  B = B + position;
  C = C + position;

  DrawTriangle(A, B, C, RED);
}

void Particle::Update() {
  Vector2 newVel = Vector2{0.98f * (velocity.x + acceleration.x),
                           0.98f * (velocity.y + acceleration.y)};
  velocity = newVel;
  position = position + velocity;
  if (dis(pGen) < 30) size -= 1.f;
}

ParticleSystem::ParticleSystem(Vector2 pos) noexcept {
  triangleVBO =
      rlLoadVertexBuffer(ParticleConstants::triangleVerts,
                         sizeof(ParticleConstants::triangleVerts), false);
  instanceVBO = rlLoadVertexBuffer(
      nullptr, ParticleConstants::kMaxParticles * sizeof(ParticleInstanceData),
      true);
  system.reserve(ParticleConstants::kMaxParticles);
  for (int i = 0; i < ParticleConstants::kMaxParticles; i++) {
    system.emplace_back(pos + Vector2{xDis(pGen), yDis(pGen)});
  }

  particleShader = LoadShader("/shader/particle.vs", "/shader/particle.fs");
}

void ParticleSystem::Draw() const {
  rlUpdateVertexBuffer(instanceVBO, instances.data(),
                       instances.size() * sizeof(ParticleInstanceData), 0);
  Matrix proj = MatrixOrtho(0.0f, (float)GetScreenWidth(),
                            (float)GetScreenHeight(), 0.0f, 0.0f, 1.0f);
  SetShaderValueMatrix(particleShader,
                       GetShaderLocation(particleShader, "projection"), proj);

  BeginShaderMode(particleShader);

  // Bind VBOs
  // rlEnableVertexArray(0);  // You may need to create a VAO
  rlDisableVertexArray();  // Unbinds any VAO
  rlDisableDepthTest();

  rlEnableVertexBuffer(triangleVBO);

  // Set up vertex attribute 0 (triangle verts)
  rlSetVertexAttribute(0, 2, RL_FLOAT, false, 2 * sizeof(float), 0);
  rlEnableVertexAttribute(0);

  rlEnableVertexBuffer(instanceVBO);
  // Set up vertex attribute 1 (position) and 2 (size)

  rlSetVertexAttribute(1, 2, RL_FLOAT, false, sizeof(ParticleInstanceData), 0);
  rlEnableVertexAttribute(1);
  rlSetVertexAttributeDivisor(1, 1);

  rlSetVertexAttribute(2, 1, RL_FLOAT, false, sizeof(ParticleInstanceData),
                       sizeof(Vector2));
  rlEnableVertexAttribute(2);
  rlSetVertexAttributeDivisor(2, 1);

  // Draw instanced
  std::cout << "Instances Size: " << instances.size() << std::endl;
  rlDrawVertexArrayInstanced(0, 3, instances.size());

  EndShaderMode();

  /*for (const auto &p : system) {
    if (p.position.x < 0 || p.position.x > GetScreenWidth() ||
        p.position.y < 0 || p.position.y > GetScreenHeight()) {
      continue;
    }
    p.Draw();
  }*/
}

void ParticleSystem::Update() {
  instances.clear();
  system.erase(std::remove_if(system.begin(), system.end(),
                              [&](auto &p) {
                                p.Update();
                                if (p.size <= 0.f || p.position.x < 0 ||
                                    p.position.x > GetScreenWidth() ||
                                    p.position.y < 0 ||
                                    p.position.y > GetScreenHeight())
                                  return true;
                                instances.push_back({p.position, p.size});
                                return false;
                                ;
                              }),
               system.end());
}