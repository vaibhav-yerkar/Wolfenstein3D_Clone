#ifndef PLAYER_H
#define PLAYER_H

#include "camera.h"
#include "custom_time.h"
#include "input.h"
#include "material.h"
#include "mesh.h"
#include "transform.h"
#include "util.h"
#include "vector2f.h"
#include "vector3f.h"
#include "window.h"
#include <random>

class Player
{
public:
  static constexpr float PLAYER_SIZE = 0.2f;

  Player(const Vector3f& pos);

  void input();
  void update();
  void render();

  void damage(int amount);
  int getDamage();
  int getHealth();
  int getMaxHealth();

  Camera& getCamera();

  Player(Player&& other) noexcept;
  Player& operator=(Player&& other) noexcept;

protected:
private:
  static Mesh* m_mesh;
  static Material* gunMaterial;
  static bool mouseLocked;

  std::mt19937 rand;
  std::random_device rd;

  bool isFiring;
  float gunFireTimer;
  int m_health;
  Transform gunTransform;
  Camera m_camera;
  Vector2f centerPosition;
  Vector3f movementVector;
};

#endif // !PLAYER_H
