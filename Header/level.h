#ifndef LEVEL_H
#define LEVEL_H

#include "bitmap.h"
#include "material.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"
#include "vertex.h"
#include <string>
#include <vector>

class Level
{
public:
  Level(const std::string& levelName, const std::string& textureName);
  void input();
  void update();
  void render();

protected:
private:
  void generateLevel();
  void addFace(std::vector<int>& indices, int startLocation, bool direction);
  std::vector<float> calcTexCoords(int value);
  void addVertices(std::vector<Vertex>& vertices, int i, int j, float offset,
                   bool x, bool y, bool z, std::vector<float> texCoord);

  Bitmap m_level;
  Mesh m_mesh;
  Shader* m_shader;
  Texture m_texture;
  Material m_material;
  Transform m_transform;
};

#endif // !LEVEL_H
