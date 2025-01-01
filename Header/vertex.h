#ifndef VERTEX_H
#define VERTEX_H

#include "vector2f.h"
#include "vector3f.h"

struct Vertex
{
  Vector3f pos;
  Vector2f texCoord;
  Vector3f normal;

  Vertex()
  {
  }

  Vertex(Vector3f pos, Vector2f texCoord = Vector2f(0, 0),
         Vector3f normal = Vector3f(0, 0, 0))
  {
    this->pos = pos;
    this->texCoord = texCoord;
    this->normal = normal;
  }
};

#endif
