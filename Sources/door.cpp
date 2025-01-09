#include "../Header/door.h"
#include "../Header/custom_time.h"
#include "../Header/game.h"
#include "../Header/mesh.h"
#include "../Header/util.h"
#include <iostream>

static const float LENGTH = 1;
static const float HEIGHT = 1;
static const float WIDTH = 0.125f;
static const float START = 0;
static const double TIME_TO_OPEN = 1.0f;
static const double CLOSE_DELAY = 2.0f;

Mesh* Door::m_mesh = nullptr;

Door::Door(Transform transform, Material* material, Vector3f openPosition)
    : m_transform(transform), m_material(material), isOpening(false),
      m_closePosition(m_transform.getPos()), m_openPosition(openPosition)
{
  if (!m_mesh)
  {
    Vertex vertices[] = {
        Vertex(Vector3f(START, START, START), Vector2f(0.5f, 1.0f)),
        Vertex(Vector3f(START, HEIGHT, START), Vector2f(0.5f, 0.75f)),
        Vertex(Vector3f(LENGTH, HEIGHT, START), Vector2f(0.75f, 0.75f)),
        Vertex(Vector3f(LENGTH, START, START), Vector2f(0.75f, 1.0f)),

        Vertex(Vector3f(START, START, START), Vector2f(0.73f, 1.0f)),
        Vertex(Vector3f(START, HEIGHT, START), Vector2f(0.73f, 0.75f)),
        Vertex(Vector3f(START, HEIGHT, WIDTH), Vector2f(0.75f, 0.75f)),
        Vertex(Vector3f(START, START, WIDTH), Vector2f(0.75f, 1.0f)),

        Vertex(Vector3f(START, START, WIDTH), Vector2f(0.5f, 1.0f)),
        Vertex(Vector3f(START, HEIGHT, WIDTH), Vector2f(0.5f, 0.75f)),
        Vertex(Vector3f(LENGTH, HEIGHT, WIDTH), Vector2f(0.75f, 0.75f)),
        Vertex(Vector3f(LENGTH, START, WIDTH), Vector2f(0.75f, 1.0f)),

        Vertex(Vector3f(LENGTH, START, START), Vector2f(0.73f, 1.0f)),
        Vertex(Vector3f(LENGTH, HEIGHT, START), Vector2f(0.73f, 0.75f)),
        Vertex(Vector3f(LENGTH, HEIGHT, WIDTH), Vector2f(0.75f, 0.75f)),
        Vertex(Vector3f(LENGTH, START, WIDTH), Vector2f(0.75f, 1.0f)),
    };

    int indices[] = {0,  1, 2, 0,  2,  3, 6,  5,  4,  7,  6,  4,
                     10, 9, 8, 11, 10, 8, 12, 13, 14, 12, 14, 15};

    m_mesh = new Mesh();
    m_mesh->addVertices(vertices, ARRAY_SIZE(vertices), indices, ARRAY_SIZE(indices));
  }
}

void Door::open()
{
  if (isOpening)
    return;

  openingStartTime = (double)Time::getTime();

  openTime = openingStartTime + TIME_TO_OPEN;
  closingStartTime = openTime + CLOSE_DELAY;
  closeTime = closingStartTime + TIME_TO_OPEN;

  isOpening = true;
}

void Door::update()
{
  if (isOpening)
  {
    double time = (double)Time::getTime();
    if (time < openTime)
    {
      getTransform().setPos(VectorLerp(m_closePosition, m_openPosition,
                                       (float)((time - openingStartTime) / TIME_TO_OPEN)));
    }
    else if (time < closingStartTime)
    {
      getTransform().setPos(m_openPosition);
    }
    else if (time < closeTime)
    {
      getTransform().setPos(VectorLerp(m_openPosition, m_closePosition,
                                       (float)((time - closingStartTime) / TIME_TO_OPEN)));
    }
    else
    {
      getTransform().setPos(m_closePosition);
      isOpening = false;
    }
  }
}

void Door::render()
{
  Shader* m_shader = Game::getLevel()->getShader();
  m_shader->updateUniforms(m_transform.getTransformation(),
                           m_transform.getProjectedTransformation(), *m_material);

  m_mesh->draw();
}

Vector2f Door::getDoorSize() const
{
  if (m_transform.getRot().getY() == 90)
    return Vector2f(WIDTH, LENGTH);
  else
    return Vector2f(LENGTH, WIDTH);
}

Transform& Door::getTransform()
{
  return m_transform;
}

Vector3f Door::VectorLerp(const Vector3f& startPos, const Vector3f& endPos, float lerpFactor)
{
  return startPos + ((endPos - startPos) * lerpFactor);
}
