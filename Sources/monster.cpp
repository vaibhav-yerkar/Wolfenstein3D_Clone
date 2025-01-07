#include "../Header/monster.h"
#include "../Header/game.h"
#include "../Header/util.h"
#include <iostream>
#include <vector>

static const float SCALE = 0.7f;
static const float SIZEY = SCALE;
static const float SIZEX = (float)((double)SIZEY / (1.9310344827586206896551724137931 * 2.0));
static const float START = 0;

static const float OFFSET_X = 0.0f;           // 0.05f
static const float OFFSET_Y = 0.0f;           // 0.01f
static const float OFFSET_FROM_GROUND = 0.0f; //-0.075f

static const float TEX_MIN_X = -OFFSET_X;
static const float TEX_MAX_X = -1 - OFFSET_X;
static const float TEX_MIN_Y = -OFFSET_Y;
static const float TEX_MAX_Y = 1 - OFFSET_Y;

static const int STATE_IDLE = 0;
static const int STATE_CHASE = 1;
static const int STATE_ATTACK = 2;
static const int STATE_DYING = 3;
static const int STATE_DEAD = 4;

static const float MOVE_SPEED = 1.0f;
static const float MOVEMENT_STOP_DISTANCE = 1.5f;
static const float MONSTER_WIDTH = 0.2f;
static const float MONSTER_LENGTH = 0.2f;

static const float SHOOT_DISTANCE = 1000.0f;
static const float SHOT_ANGLE = 10.0f;
static const float ATTACK_CHANCE = 0.75f;
static const int MAX_HEALTH = 100;
static const int DAMAGE_MIN = 5;
static const int DAMAGE_MAX = 30;

Mesh* Monster::m_mesh = nullptr;
std::vector<Texture*> Monster::m_animations;

Monster::Monster(const Transform& transform)
    : m_transform(transform), state(STATE_IDLE), health(MAX_HEALTH), canLook(false),
      canAttack(false), deathTime(0), m_rand(std::random_device{}())
{
  if (m_animations.empty())
  {
    m_animations.push_back(new Texture("SSWVA1.png"));
    m_animations.push_back(new Texture("SSWVB1.png"));
    m_animations.push_back(new Texture("SSWVC1.png"));
    m_animations.push_back(new Texture("SSWVD1.png"));

    m_animations.push_back(new Texture("SSWVE0.png"));
    m_animations.push_back(new Texture("SSWVF0.png"));
    m_animations.push_back(new Texture("SSWVG0.png"));

    m_animations.push_back(new Texture("SSWVH0.png"));

    m_animations.push_back(new Texture("SSWVI0.png"));
    m_animations.push_back(new Texture("SSWVJ0.png"));
    m_animations.push_back(new Texture("SSWVK0.png"));
    m_animations.push_back(new Texture("SSWVL0.png"));

    m_animations.push_back(new Texture("SSWVM0.png"));
  }

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

  this->m_transform = transform;
}

Transform& Monster::getTransform()
{
  return m_transform;
}

Vector2f Monster::getSize() const
{
  return Vector2f(MONSTER_WIDTH, MONSTER_LENGTH);
}

void Monster::damage(int amount)
{
  if (state == STATE_IDLE)
  {
    state = STATE_CHASE;
  }
  health -= amount;
  if (health <= 0)
  {
    state = STATE_DYING;
  }
}

void Monster::idleUpdate(const Vector3f& orientation, float direction)
{
  double time = (double)Time::getTime() / (double)Time::SECOND;
  double timeDecimals = time - (int)time;

  if (timeDecimals < 0.0000000005)
  {
    canLook = true;
    m_material.setTexture(m_animations[0]);
  }
  else
  {
    m_material.setTexture(m_animations[1]);
    if (direction < SHOOT_DISTANCE)
    {
      Vector2f lineStart(m_transform.getPos().getX(), m_transform.getPos().getZ());
      Vector2f castDirection(orientation.getX(), orientation.getZ());
      Vector2f lineEnd = lineStart + castDirection * SHOOT_DISTANCE;

      Vector2f collisionVector = Game::getLevel()->checkIntersection(lineStart, lineEnd, false);
      Vector2f playerIntersectVector(Transform::getCamera().getPos().getX(),
                                     Transform::getCamera().getPos().getZ());

      if (collisionVector.length() == 0 ||
          (playerIntersectVector - lineStart).length() < (collisionVector - lineStart).length())
      {
        state = STATE_CHASE;
      }
      canLook = false;
    }
  }
}

void Monster::chaseUpdate(const Vector3f& orientation, float distance)
{
  double time = (double)Time::getTime() / (double)Time::SECOND;
  double timeDecimals = time - (int)time;

  if (timeDecimals < 0.0000000025)
  {
    m_material.setTexture(m_animations[0]);
  }
  else if (timeDecimals < 0.000000005)
  {
    m_material.setTexture(m_animations[1]);
  }
  else if (timeDecimals < 0.0000000075)
  {
    m_material.setTexture(m_animations[2]);
  }
  else
  {
    m_material.setTexture(m_animations[3]);
  }

  // if (std::generate_canonical<double, 10000000000>(m_rand) < ATTACK_CHANCE * Time::getDelta())
  if (distance <= MOVEMENT_STOP_DISTANCE)
  {
    state = STATE_ATTACK;
  }

  if (distance > MOVEMENT_STOP_DISTANCE)
  {
    float moveAmount = MOVE_SPEED * Time::getDelta();

    Vector3f oldPos = m_transform.getPos();
    Vector3f newPos = m_transform.getPos() + orientation * moveAmount;

    Vector3f collisionVector =
        Game::getLevel()->checkCollision(oldPos, newPos, MONSTER_WIDTH, MONSTER_LENGTH);
    Vector3f movementVector = collisionVector.mult(orientation);

    if (movementVector.length() > 0)
    {
      m_transform.setPos(m_transform.getPos() + movementVector * moveAmount);
    }

    if ((movementVector - orientation).length() != 0)
    {
      Game::getLevel()->openDoors(m_transform.getPos(), false);
    }
  }
  else
  {
    state = STATE_ATTACK;
  }
}

void Monster::attackUpdate(const Vector3f& orientation, float distance)
{
  double time = (double)Time::getTime() / (double)Time::SECOND;
  double timeDecimals = time - static_cast<int>(time);

  if (timeDecimals < 0.000000005)
  {
    m_material.setTexture(m_animations[4]);
  }
  else if (timeDecimals < 0.0000000075)
  {
    m_material.setTexture(m_animations[5]);
  }
  else if (timeDecimals < 0.000000009)
  {
    m_material.setTexture(m_animations[6]);

    if (canAttack)
    {
      Vector2f lineStart(m_transform.getPos().getX(), m_transform.getPos().getZ());
      Vector2f castDirection(orientation.getX(), orientation.getZ());
      castDirection =
          castDirection.rotate((std::generate_canonical<float, 10>(m_rand) - 0.5f) * SHOT_ANGLE);
      Vector2f lineEnd = lineStart + castDirection * SHOOT_DISTANCE;

      Vector2f collisionVector = Game::getLevel()->checkIntersection(lineStart, lineEnd, false);
      Vector2f playerIntersectVector = Game::getLevel()->lineIntersectRect(
          lineStart, lineEnd,
          Vector2f(Transform::getCamera().getPos().getX(), Transform::getCamera().getPos().getZ()),
          Vector2f(Player::PLAYER_SIZE, Player::PLAYER_SIZE));

      if ((playerIntersectVector - lineStart).length() == 0 ||
          (collisionVector.length() == 0 ||
           (playerIntersectVector - lineStart).length() < (collisionVector - lineStart).length()))
      {
        Game::getLevel()->damagePlayer(m_rand() % (DAMAGE_MAX - DAMAGE_MIN + 1) + DAMAGE_MIN);
      }

      canAttack = false;
    }
  }
  else
  {
    m_material.setTexture(m_animations[5]);
    state = STATE_CHASE;
    canAttack = true;
  }
}

void Monster::dyingUpdate(const Vector3f& orientation, float distance)
{
  double time = (double)Time::getTime() / (double)Time::SECOND;

  if (deathTime == 0)
  {
    deathTime = time;
  }

  float time1 = 0.0000000001f;
  float time2 = 0.0000000003f;
  float time3 = 0.00000000045f;
  float time4 = 0.0000000006f;

  if (time < deathTime + time1)
  {
    m_material.setTexture(m_animations[8]);
    m_transform.setScale(Vector3f(1, 0.9642857142857142f, 1));
  }
  else if (time < deathTime + time2)
  {
    m_material.setTexture(m_animations[9]);
    m_transform.setScale(Vector3f(1.7f, 0.9f, 1));
  }
  else if (time < deathTime + time3)
  {
    m_material.setTexture(m_animations[10]);
    m_transform.setScale(Vector3f(1.7f, 0.9f, 1));
  }
  else if (time < deathTime + time4)
  {
    m_material.setTexture(m_animations[11]);
    m_transform.setScale(Vector3f(1.7f, 0.5f, 1));
  }
  else
  {
    state = STATE_DEAD;
  }
}

void Monster::deadUpdate(const Vector3f& orientation, float distance)
{
  m_material.setTexture(m_animations[12]);
  m_transform.setScale(Vector3f(1.7586206896551724f, 0.2857142857142857f, 1));
}

void Monster::alignWithGround()
{
  m_transform.getPos().setY(OFFSET_FROM_GROUND);
}

void Monster::faceCamera(const Vector3f& directionToCamera)
{
  float angleToFaceTheCamera =
      std::atan2(directionToCamera.getZ(), directionToCamera.getX()) * 180.0f / M_PI;

  if (directionToCamera.getX() < 0)
  {
    angleToFaceTheCamera += 360;
  }

  m_transform.getRot().setY(angleToFaceTheCamera + 90);
}

void Monster::update()
{
  Vector3f directionToCamera = Transform::getCamera().getPos() - m_transform.getPos();

  float distance = directionToCamera.length();
  Vector3f orientation = directionToCamera / distance;

  alignWithGround();
  faceCamera(orientation);

  switch (state)
  {
  case STATE_IDLE:
    idleUpdate(orientation, distance);
    break;
  case STATE_CHASE:
    chaseUpdate(orientation, distance);
    break;
  case STATE_ATTACK:
    attackUpdate(orientation, distance);
    break;
  case STATE_DYING:
    dyingUpdate(orientation, distance);
    break;
  case STATE_DEAD:
    deadUpdate(orientation, distance);
    break;
  }
}

void Monster::render()
{
  Shader* shader = Game::getLevel()->getShader();
  shader->updateUniforms(m_transform.getTransformation(), m_transform.getProjectedTransformation(),
                         m_material);
  m_mesh->draw();
}
