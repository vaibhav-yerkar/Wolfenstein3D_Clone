#ifndef LEVEL_H
#define LEVEL_H

#include "bitmap.h"
#include "door.h"
#include "material.h"
#include "mesh.h"
#include "monster.h"
#include "player.h"
#include "shader.h"
#include "transform.h"
#include "vector2f.h"
#include "vector3f.h"
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

  Vector3f checkCollision(Vector3f& oldPos, Vector3f& newPos, float objWidth, float objLength);
  Vector2f checkIntersection(const Vector2f& lineStart, const Vector2f& lineEnd, bool hurtMonster);
  Vector2f lineIntersectRect(const Vector2f& lineStart, const Vector2f& lineEnd,
                             const Vector2f& rectPos, const Vector2f& rectSize);

  Player* getPlayer();
  Shader* getShader();

  void openDoors(Vector3f pos, bool tryExitLevel);
  void damagePlayer(int amount);

protected:
private:
  std::vector<float> calcTexCoords(int value);

  Vector2f findNearestVector2f(const Vector2f& a, const Vector2f& b,
                               const Vector2f& positionRelativeTo);
  Vector2f lineIntersect(const Vector2f& lineStart1, const Vector2f& lineEnd1,
                         const Vector2f& lineStart2, const Vector2f& lineEnd2);
  Vector2f rectCollide(Vector2f oldPos, Vector2f newPos, Vector2f size1, Vector2f pos2,
                       Vector2f size2);

  float Vector2fCross(const Vector2f& a, const Vector2f& b);

  void addVertices(std::vector<Vertex>& vertices, int i, int j, float offset, bool x, bool y,
                   bool z, std::vector<float> texCoord);
  void addFace(std::vector<int>& indices, int startLocation, bool direction);
  void addSpecial(int blueValue, int x, int y);
  void addDoor(int x, int y);
  void generateLevel();

  std::vector<Door*> m_doors;
  std::vector<Monster*> m_monsters;
  std::vector<Vector3f> exitPoints;
  std::vector<Vector2f> collisionPosStart;
  std::vector<Vector2f> collisionPosEnd;

  Transform m_transform;
  Player m_player;

  Mesh m_mesh;
  Bitmap m_level;
  Texture m_texture;
  Shader* m_shader;
  Material m_material;
};

#endif // !LEVEL_H
