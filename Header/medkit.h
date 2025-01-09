
#ifndef MEDKIT_H
#define MEDKIT_H

#include "material.h"
#include "mesh.h"
#include "player.h"
#include "shader.h"
#include "transform.h"
#include "util.h"
#include "vector3f.h"

class Medkit
{
public:
  static constexpr float PICKUP_DISTANCE = 0.75f;
  static constexpr int HEAL_AMOUNT = 25;

  static constexpr float SCALE = 0.25f;
  static constexpr float SIZEY = SCALE;
  static constexpr float SIZEX =
      static_cast<float>(SIZEY / (0.67857142857142857142857142857143 * 2.5));
  static constexpr float START = 0;

  static constexpr float OFFSET_X = 0.0f;
  static constexpr float OFFSET_Y = 0.0f;

  static constexpr float TEX_MIN_X = -OFFSET_X;
  static constexpr float TEX_MAX_X = -1.0f - OFFSET_X;
  static constexpr float TEX_MIN_Y = -OFFSET_Y;
  static constexpr float TEX_MAX_Y = 1.0f - OFFSET_Y;

  explicit Medkit(const Vector3f& position);

  void update();
  void render();

private:
  static Mesh* m_mesh;
  static Material* m_material;

  Transform transform;
};

#endif // MEDKIT_H
