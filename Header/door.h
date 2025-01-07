#ifndef DOOR_H
#define DOOR_H

#include "material.h"
#include "mesh.h"
#include "shader.h"
#include "transform.h"
#include "vector2f.h"
#include "vector3f.h"

class Door
{
public:
  Door()
      : m_closePosition(Vector3f(0, 0, 0)), isOpening(false), openingStartTime(0), openTime(0),
        closingStartTime(0), closeTime(0)
  {
  }
  Door(Transform transform, Material* material, Vector3f openPosition);

  void open();
  void update();
  void render();

  Vector2f getDoorSize() const;
  Transform& getTransform();

protected:
private:
  Vector3f VectorLerp(const Vector3f& startPos, const Vector3f& endPos, float lerpFactor);

  static Mesh* m_mesh;
  Transform m_transform;

  Vector3f m_openPosition;
  Vector3f m_closePosition;

  Material* m_material;

  bool isOpening;
  double openingStartTime;
  double openTime;
  double closingStartTime;
  double closeTime;
};

#endif // !DOOR_H
