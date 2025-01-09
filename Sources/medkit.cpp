#include "medkit.h"
#include "../Header/game.h"
#include "../Header/player.h"
#include "../Header/shader.h"
#include "../Header/util.h"

Mesh* Medkit::m_mesh = nullptr;
Material* Medkit::m_material = nullptr;

Medkit::Medkit(const Vector3f& position)
{
  if (!m_mesh)
  {
    Vertex vertices[] = {Vertex(Vector3f(-SIZEX, START, START), Vector2f(TEX_MAX_X, TEX_MAX_Y)),
                         Vertex(Vector3f(-SIZEX, SIZEY, START), Vector2f(TEX_MAX_X, TEX_MIN_Y)),
                         Vertex(Vector3f(SIZEX, SIZEY, START), Vector2f(TEX_MIN_X, TEX_MIN_Y)),
                         Vertex(Vector3f(SIZEX, START, START), Vector2f(TEX_MIN_X, TEX_MAX_Y))};

    int indices[] = {0, 1, 2, 0, 2, 3};

    m_mesh = new Mesh();
    m_mesh->addVertices(vertices, ARRAY_SIZE(vertices), indices, ARRAY_SIZE(indices));
  }

  if (!m_material)
  {
    m_material = new Material(new Texture("MEDIA0.png"));
  }

  transform.setPos(position);
}

void Medkit::update()
{
  Vector3f directionToCamera = Transform::getCamera().getPos() - transform.getPos();

  float angleToFaceTheCamera = static_cast<float>(
      std::atan2(directionToCamera.getZ(), directionToCamera.getX()) * 180.0 / M_PI);

  if (directionToCamera.getX() < 0)
    angleToFaceTheCamera += 360;

  transform.getRot().setY(angleToFaceTheCamera + 90);

  if (directionToCamera.length() < PICKUP_DISTANCE)
  {
    Player* player = Game::getLevel()->getPlayer();

    if (player->getHealth() < player->getMaxHealth())
    {
      Game::getLevel()->removeMedkit(this);
      player->damage(-HEAL_AMOUNT); // Negative damage heals the player
    }
  }
}

void Medkit::render()
{
  Shader* shader = Game::getLevel()->getShader();
  shader->updateUniforms(transform.getTransformation(), transform.getProjectedTransformation(),
                         *m_material);
  m_mesh->draw();
}
