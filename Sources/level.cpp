#include "../Header/level.h"
#include "../Header/basicShader.h"
#include "../Header/texture.h"
#include <cstdlib>
#include <iostream>
#include <vector>

const static float SPOT_WIDTH = 1.0f;
const static float SPOT_LENGTH = 1.0f;
const static float SPOT_HEIGHT = 1.0f;
const static int NUM_TEXTURE_EXP = 4;
const static int NUM_TEXTURE = (int)pow(2, NUM_TEXTURE_EXP);

Level::Level(const std::string& levelName, const std::string& textureName) : m_level(levelName)
{
  m_level = m_level.flipY();
  m_texture = Texture(textureName);
  m_material = Material(&m_texture);

  m_shader = BasicShader::getInstance();

  generateLevel();
}

void Level::input()
{
}

void Level::update()
{
}

void Level::render()
{
  m_shader->bind();
  m_shader->updateUniforms(m_transform.getTransformation(),
                           m_transform.getProjectedTransformation(), m_material);
  m_mesh.draw();
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
          collisionVector = collisionVector * rectCollide(oldPos2, newPos2, objectSize,
                                                          blockSize * Vector2f(i, j), blockSize);
        }
      }
    }
  }

  return Vector3f(collisionVector.getX(), 0, collisionVector.getY());
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
        addFace(indices, vertices.size(), false);
        addVertices(vertices, i, 0, j, true, true, false, texCoord);
      }
      if ((m_level.getPixel(i, j + 1) & 0xFFFFFF) == 0)
      {
        addFace(indices, vertices.size(), true);
        addVertices(vertices, i, 0, (j + 1), true, true, false, texCoord);
      }
      if ((m_level.getPixel(i - 1, j) & 0xFFFFFF) == 0)
      {
        addFace(indices, vertices.size(), true);
        addVertices(vertices, 0, j, i, false, true, true, texCoord);
      }
      if ((m_level.getPixel(i + 1, j) & 0xFFFFFF) == 0)
      {
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
