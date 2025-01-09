#include "../Header/level.h"
#include "../Header/basicShader.h"
#include "../Header/game.h"
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <vector>

const static float SPOT_WIDTH = 1.0f;
const static float SPOT_LENGTH = 1.0f;
const static float SPOT_HEIGHT = 1.0f;
const static float OPEN_DISTANCE = 1.0f;
const static float DOOR_OPEN_MOVEMENT_AMOUNT = 0.9f;
const static int NUM_TEXTURE_EXP = 4;
const static int NUM_TEXTURE = (int)pow(2, NUM_TEXTURE_EXP);

Level::Level(const std::string& levelName, const std::string& textureName)
    : m_transform(), m_player(Player(Vector3f(7, 0.435, 7))), m_level(levelName)
{
  m_level.flipY();
  m_texture = Texture(textureName);
  m_material = Material(&m_texture);
  m_shader = BasicShader::getInstance();
  generateLevel();
}

void Level::openDoors(Vector3f pos, bool tryExitLevel)
{
  for (Door* door : m_doors)
  {
    if ((door->getTransform().getPos() - pos).length() < OPEN_DISTANCE)
      door->open();
  }
  if (tryExitLevel)
  {
    for (Vector3f exitPoint : exitPoints)
    {
      if ((exitPoint - pos).length() < OPEN_DISTANCE)
      {
        Game::loadNextLevel();
      }
    }
  }
}

void Level::damagePlayer(int amount)
{
  m_player.damage(amount);
}

void Level::input()
{
  m_player.input();
}

void Level::update()
{
  for (Door* door : m_doors)
    door->update();

  for (Monster* monster : m_monsters)
    monster->update();
  for (Medkit& medkit : m_medkits)
    medkit.update();
  m_medkits.erase(std::remove_if(m_medkits.begin(), m_medkits.end(),
                                 [this](const Medkit& medkit)
                                 {
                                   return std::find(m_medkitsToRemove.begin(),
                                                    m_medkitsToRemove.end(),
                                                    &medkit) != m_medkitsToRemove.end();
                                 }),
                  m_medkits.end());
  m_player.update();
}

void Level::render()
{
  m_shader->bind();
  m_shader->updateUniforms(m_transform.getTransformation(),
                           m_transform.getProjectedTransformation(), m_material);
  m_mesh.draw();

  for (Door* door : m_doors)
  {
    door->render();
  }
  for (Monster* monster : m_monsters)
  {
    monster->render();
  }
  for (Medkit& medkit : m_medkits)
  {
    medkit.render();
  }
  m_player.render();
}

Player* Level::getPlayer()
{
  return &m_player;
}

Shader* Level::getShader()
{
  return m_shader;
}

void Level::addSpecial(int blueValue, int x, int y)
{
  if (blueValue == 16)
    addDoor(x, y);
  if (blueValue == 1)
    m_player = Player(Vector3f((x + 0.5f) * SPOT_WIDTH, 0.4375f, (y + 0.5f) * SPOT_LENGTH));
  if (blueValue == 128)
  {
    Transform monsterTransform = Transform();
    monsterTransform.setPos(Vector3f((x + 0.5f) * SPOT_WIDTH, 0, (y + 0.5f) * SPOT_LENGTH));
    m_monsters.push_back(new Monster(monsterTransform));
  }
  if (blueValue == 192)
    m_medkits.push_back(Medkit(Vector3f((x + 0.5f) * SPOT_WIDTH, 0, (y + 0.5f) * SPOT_LENGTH)));
  if (blueValue == 97)
    exitPoints.push_back(Vector3f((x + 0.5f) * SPOT_WIDTH, 0, (y + 0.5f) * SPOT_LENGTH));
}

void Level::generateLevel()
{
  std::vector<int> indices;
  std::vector<Vertex> vertices;

  for (int i = 0; i < m_level.getWidth(); i++)
  {
    for (int j = 0; j < m_level.getHeight(); j++)
    {
      if ((m_level.getPixel(i, j) & 0xFFFFFF) == 0)
        continue;

      addSpecial(((m_level.getPixel(i, j) & 0xFF0000) >> 16), i, j);

      std::vector<float> texCoord = calcTexCoords((m_level.getPixel(i, j) & 0x00FF00) >> 8);

      // Generate Floor
      addFace(indices, vertices.size(), true);
      addVertices(vertices, i, j, 0, true, false, true, texCoord);

      // Generate Ceiling
      addFace(indices, vertices.size(), false);
      addVertices(vertices, i, j, 1, true, false, true, texCoord);

      texCoord = calcTexCoords((m_level.getPixel(i, j) & 0xFF0000) >> 16);

      // Generate Walls
      if ((m_level.getPixel(i, j - 1) & 0xFFFFFF) == 0)
      {
        collisionPosStart.push_back(Vector2f(i * SPOT_WIDTH, j * SPOT_LENGTH));
        collisionPosEnd.push_back(Vector2f((i + 1) * SPOT_WIDTH, j * SPOT_LENGTH));
        addFace(indices, vertices.size(), false);
        addVertices(vertices, i, 0, j, true, true, false, texCoord);
      }
      if ((m_level.getPixel(i, j + 1) & 0xFFFFFF) == 0)
      {
        collisionPosStart.push_back(Vector2f(i * SPOT_WIDTH, (j + 1) * SPOT_LENGTH));
        collisionPosEnd.push_back(Vector2f((i + 1) * SPOT_WIDTH, (j + 1) * SPOT_LENGTH));
        addFace(indices, vertices.size(), true);
        addVertices(vertices, i, 0, (j + 1), true, true, false, texCoord);
      }
      if ((m_level.getPixel(i - 1, j) & 0xFFFFFF) == 0)
      {
        collisionPosStart.push_back(Vector2f(i * SPOT_WIDTH, j * SPOT_LENGTH));
        collisionPosEnd.push_back(Vector2f(i * SPOT_WIDTH, (j + 1) * SPOT_LENGTH));
        addFace(indices, vertices.size(), true);
        addVertices(vertices, 0, j, i, false, true, true, texCoord);
      }
      if ((m_level.getPixel(i + 1, j) & 0xFFFFFF) == 0)
      {
        collisionPosStart.push_back(Vector2f((i + 1) * SPOT_WIDTH, j * SPOT_LENGTH));
        collisionPosEnd.push_back(Vector2f((i + 1) * SPOT_WIDTH, (j + 1) * SPOT_LENGTH));
        addFace(indices, vertices.size(), false);
        addVertices(vertices, 0, j, (i + 1), false, true, true, texCoord);
      }
    }
  }

  Vertex* vertArray = vertices.data();
  int* intArray = indices.data();
  m_mesh.addVertices(vertArray, vertices.size(), intArray, indices.size());
}

Vector2f Level::rectCollide(Vector2f oldPos, Vector2f newPos, Vector2f size1, Vector2f pos2,
                            Vector2f size2)
{
  Vector2f res = Vector2f(0, 0);

  if (newPos.getX() + size1.getX() < pos2.getX() ||
      newPos.getX() - size1.getX() > pos2.getX() + size2.getX() * size2.getX() ||
      oldPos.getY() + size1.getY() < pos2.getY() ||
      oldPos.getY() - size1.getY() > pos2.getY() + size2.getY() * size2.getY())
    res.setX(1);

  if (oldPos.getX() + size1.getX() < pos2.getX() ||
      oldPos.getX() - size1.getX() > pos2.getX() + size2.getX() * size2.getX() ||
      newPos.getY() + size1.getY() < pos2.getY() ||
      newPos.getY() - size1.getY() > pos2.getY() + size2.getY() * size2.getY())
    res.setY(1);

  return res;
}

Vector3f Level::checkCollision(Vector3f& oldPos, Vector3f& newPos, float objWidth, float objLength)
{
  Vector2f collisionVector(1, 1);
  Vector3f movementVector = newPos - oldPos;

  if (movementVector.length() > 0)
  {
    Vector2f blockSize(SPOT_WIDTH, SPOT_LENGTH);
    Vector2f objectSize(objWidth, objLength);

    Vector2f oldPos2(oldPos.getX(), oldPos.getZ());
    Vector2f newPos2(newPos.getX(), newPos.getZ());

    for (int i = 0; i < m_level.getWidth(); i++)
    {
      for (int j = 0; j < m_level.getHeight(); j++)
      {
        if ((m_level.getPixel(i, j) & 0xFFFFFF) == 0)
        {
          collisionVector = collisionVector.mult(
              rectCollide(oldPos2, newPos2, objectSize, blockSize.mult(Vector2f(i, j)), blockSize));
        }
      }
    }
    for (Door* door : m_doors)
    {
      Vector2f doorSize = door->getDoorSize();
      Vector3f doorPos3f = door->getTransform().getPos();
      Vector2f doorPos2f = Vector2f(doorPos3f.getX(), doorPos3f.getZ());
      collisionVector =
          collisionVector.mult(rectCollide(oldPos2, newPos2, objectSize, doorPos2f, doorSize));
    }
  }

  return Vector3f(collisionVector.getX(), 0, collisionVector.getY());
}

Vector2f Level::checkIntersection(const Vector2f& lineStart, const Vector2f& lineEnd,
                                  bool hurtMonster)
{
  Vector2f nearestIntersection = NULL;
  for (int i = 0; i < collisionPosStart.size(); i++)
  {
    Vector2f collisionVector =
        lineIntersect(lineStart, lineEnd, collisionPosStart[i], collisionPosEnd[i]);
    nearestIntersection = findNearestVector2f(nearestIntersection, collisionVector, lineStart);
  }
  for (Door* door : m_doors)
  {
    Vector2f doorSize = door->getDoorSize();
    Vector3f doorPos3f = door->getTransform().getPos();
    Vector2f doorPos2f = Vector2f(doorPos3f.getX(), doorPos3f.getZ());
    Vector2f collisionVector = lineIntersectRect(lineStart, lineEnd, doorPos2f, doorSize);

    nearestIntersection = findNearestVector2f(nearestIntersection, collisionVector, lineStart);
  }
  if (hurtMonster)
  {
    Vector2f nearestMonsterIntersect = NULL;
    Monster* nearestMonster = nullptr;

    for (Monster* monster : m_monsters)
    {
      Vector2f monsterSize = monster->getSize();
      Vector3f monsterPos3f = monster->getTransform().getPos();
      Vector2f monsterPos2f = Vector2f(monsterPos3f.getX(), monsterPos3f.getZ());
      Vector2f collisionVector = lineIntersectRect(lineStart, lineEnd, monsterPos2f, monsterSize);

      nearestMonsterIntersect =
          findNearestVector2f(nearestMonsterIntersect, collisionVector, lineStart);

      if (nearestMonsterIntersect == collisionVector)
        nearestMonster = monster;
    }

    if (nearestMonsterIntersect != NULL &&
        (nearestIntersection == NULL || (nearestMonsterIntersect - lineStart).length() <
                                            (nearestIntersection - lineStart).length()))
    {
      if (nearestMonster != nullptr)
        nearestMonster->damage(m_player.getDamage());
    }
  }
  return nearestIntersection;
}

void Level::addFace(std::vector<int>& indices, int startLocation, bool direction)
{
  if (direction)
  {
    indices.push_back(startLocation + 2);
    indices.push_back(startLocation + 1);
    indices.push_back(startLocation + 0);
    indices.push_back(startLocation + 3);
    indices.push_back(startLocation + 2);
    indices.push_back(startLocation + 0);
  }
  else
  {
    indices.push_back(startLocation + 0);
    indices.push_back(startLocation + 1);
    indices.push_back(startLocation + 2);
    indices.push_back(startLocation + 0);
    indices.push_back(startLocation + 2);
    indices.push_back(startLocation + 3);
  }
}

std::vector<float> Level::calcTexCoords(int value)
{
  int texX = value / NUM_TEXTURE;
  int texY = texX % NUM_TEXTURE_EXP;
  texX /= NUM_TEXTURE_EXP;

  std::vector<float> result(4);

  result[0] = 1.0f - (float)texX / (float)NUM_TEXTURE_EXP;
  result[1] = result[0] - 1.0f / (float)NUM_TEXTURE_EXP;
  result[3] = 1.0f - (float)texY / (float)NUM_TEXTURE_EXP;
  result[2] = result[3] - 1.0f / (float)NUM_TEXTURE_EXP;

  return result;
}

void Level::addVertices(std::vector<Vertex>& vertices, int i, int j, float offset, bool x, bool y,
                        bool z, std::vector<float> texCoord)
{
  if (x && z)
  {
    vertices.push_back(Vertex(Vector3f(i * SPOT_WIDTH, offset * SPOT_HEIGHT, j * SPOT_LENGTH),
                              Vector2f(texCoord[1], texCoord[3])));
    vertices.push_back(Vertex(Vector3f((i + 1) * SPOT_WIDTH, offset * SPOT_HEIGHT, j * SPOT_LENGTH),
                              Vector2f(texCoord[0], texCoord[3])));
    vertices.push_back(
        Vertex(Vector3f((i + 1) * SPOT_WIDTH, offset * SPOT_HEIGHT, (j + 1) * SPOT_LENGTH),
               Vector2f(texCoord[0], texCoord[2])));
    vertices.push_back(Vertex(Vector3f(i * SPOT_WIDTH, offset * SPOT_HEIGHT, (j + 1) * SPOT_LENGTH),
                              Vector2f(texCoord[1], texCoord[2])));
  }
  else if (x && y)
  {
    vertices.push_back(Vertex(Vector3f(i * SPOT_WIDTH, j * SPOT_HEIGHT, offset * SPOT_LENGTH),
                              Vector2f(texCoord[1], texCoord[3])));
    vertices.push_back(Vertex(Vector3f((i + 1) * SPOT_WIDTH, j * SPOT_HEIGHT, offset * SPOT_LENGTH),
                              Vector2f(texCoord[0], texCoord[3])));
    vertices.push_back(
        Vertex(Vector3f((i + 1) * SPOT_WIDTH, (j + 1) * SPOT_HEIGHT, offset * SPOT_LENGTH),
               Vector2f(texCoord[0], texCoord[2])));
    vertices.push_back(Vertex(Vector3f(i * SPOT_WIDTH, (j + 1) * SPOT_HEIGHT, offset * SPOT_LENGTH),
                              Vector2f(texCoord[1], texCoord[2])));
  }
  else if (y && z)
  {
    vertices.push_back(Vertex(Vector3f(offset * SPOT_WIDTH, i * SPOT_HEIGHT, j * SPOT_LENGTH),
                              Vector2f(texCoord[1], texCoord[3])));
    vertices.push_back(Vertex(Vector3f(offset * SPOT_WIDTH, i * SPOT_HEIGHT, (j + 1) * SPOT_LENGTH),
                              Vector2f(texCoord[0], texCoord[3])));
    vertices.push_back(
        Vertex(Vector3f(offset * SPOT_WIDTH, (i + 1) * SPOT_HEIGHT, (j + 1) * SPOT_LENGTH),
               Vector2f(texCoord[0], texCoord[2])));
    vertices.push_back(Vertex(Vector3f(offset * SPOT_WIDTH, (i + 1) * SPOT_HEIGHT, j * SPOT_LENGTH),
                              Vector2f(texCoord[1], texCoord[2])));
  }
  else
  {
    std::cerr << "Invalid plane generation" << std::endl;
    exit(1);
  }
}

void Level::addDoor(int x, int y)
{
  Transform doorTransform = Transform();

  bool xDoor =
      (m_level.getPixel(x, y - 1) & 0xFFFFFF) == 0 && (m_level.getPixel(x, y + 1) & 0xFFFFFF) == 0;
  bool yDoor =
      (m_level.getPixel(x - 1, y) & 0xFFFFFF) == 0 && (m_level.getPixel(x + 1, y) & 0xFFFFFF) == 0;
  if (!(xDoor ^ yDoor))
  {
    std::cerr << "Level Generation failed : Placed doors at invalid position : " << x << ", " << y
              << std::endl;
    exit(1);
  }
  Vector3f openPosition = NULL;

  if (yDoor)
  {
    doorTransform.setPos(Vector3f(x, 0, y + SPOT_LENGTH / 2));
    openPosition = doorTransform.getPos() - Vector3f(DOOR_OPEN_MOVEMENT_AMOUNT, 0.0f, 0.0f);
  }
  if (xDoor)
  {
    doorTransform.setPos(Vector3f(x + SPOT_WIDTH / 2, 0, y));
    doorTransform.setRot(Vector3f(0, 90, 0));
    openPosition = doorTransform.getPos() - Vector3f(0.0f, 0.0f, DOOR_OPEN_MOVEMENT_AMOUNT);
  }
  m_doors.push_back(new Door(doorTransform, &m_material, openPosition));
}

Vector2f Level::findNearestVector2f(const Vector2f& a, const Vector2f& b,
                                    const Vector2f& positionRelativeTo)
{
  if (b != NULL &&
      (a == NULL || ((a - positionRelativeTo).length() > (b - positionRelativeTo).length())))
    return b;

  return a;
}

Vector2f Level::lineIntersectRect(const Vector2f& lineStart, const Vector2f& lineEnd,
                                  const Vector2f& rectPos, const Vector2f& rectSize)
{
  Vector2f result = NULL;

  Vector2f collisionVector = lineIntersect(
      lineStart, lineEnd, rectPos, Vector2f(rectPos.getX() + rectSize.getX(), rectPos.getY()));
  result = findNearestVector2f(result, collisionVector, lineStart);

  collisionVector = lineIntersect(lineStart, lineEnd, rectPos,
                                  Vector2f(rectPos.getX(), rectPos.getY() + rectSize.getY()));
  result = findNearestVector2f(result, collisionVector, lineStart);

  collisionVector =
      lineIntersect(lineStart, lineEnd, Vector2f(rectPos.getX(), rectPos.getY() + rectSize.getY()),
                    rectPos + rectSize);
  result = findNearestVector2f(result, collisionVector, lineStart);

  collisionVector =
      lineIntersect(lineStart, lineEnd, Vector2f(rectPos.getX() + rectSize.getX(), rectPos.getY()),
                    rectPos + rectSize);
  result = findNearestVector2f(result, collisionVector, lineStart);

  return result;
}

Vector2f Level::lineIntersect(const Vector2f& lineStart1, const Vector2f& lineEnd1,
                              const Vector2f& lineStart2, const Vector2f& lineEnd2)
{
  Vector2f line1 = lineEnd1 - lineStart1;
  Vector2f line2 = lineEnd2 - lineStart2;

  float cross = Vector2fCross(line1, line2);

  if (cross == 0)
    return NULL;

  Vector2f distanceBetweenLineStarts = lineStart2 - lineStart1;

  float a = Vector2fCross(distanceBetweenLineStarts, line2) / cross;
  float b = Vector2fCross(distanceBetweenLineStarts, line1) / cross;

  if (0.0f < a && a < 1.0f && 0.0f < b && b < 1.0f)
    return lineStart1 + line1.mult(a);

  return NULL;
}

float Level::Vector2fCross(const Vector2f& a, const Vector2f& b)
{
  return a.getX() * b.getY() - a.getY() * b.getX();
}

void Level::removeMedkit(Medkit* medkit)
{
  m_medkitsToRemove.emplace_back(medkit);
}
