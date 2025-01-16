#ifndef MONSTER_H
#define MONSTER_H

#include "custom_time.h"
#include "material.h"
#include "matrix4f.h"
#include "mesh.h"
#include "texture.h"
#include "transform.h"
#include "util.h"
#include "vector2f.h"
#include "vector3f.h"

#include <random>
#include <vector>

class Monster
{
public:
  Monster(const Transform& transform);

  Transform& getTransform();
  Vector2f getSize() const;
  void damage(int amount);
  void update();
  void render();

protected:
private:
  void idleUpdate(const Vector3f& orientation, float distance);
  void chaseUpdate(const Vector3f& orientation, float distance);
  void attackUpdate(const Vector3f& orientation, float distance);
  void dyingUpdate(const Vector3f& orientation, float distance);
  void deadUpdate(const Vector3f& orientation, float distance);
  void alignWithGround();
  void faceCamera(const Vector3f& directionToCamera);

  static Mesh* m_mesh;
  static std::vector<Texture*> m_animations;

  Material m_material;
  Transform m_transform;
  std::mt19937 m_rand;
  int state;
  int health;
  bool canLook;
  bool canAttack;
  double deathTime;
};

#endif // !MONSTER_H
