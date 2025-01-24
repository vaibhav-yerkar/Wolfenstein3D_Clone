#include "../Header/player.h"
#include "../Header/game.h"
#include "../Header/level.h"
#include "window.h"
#include <iostream>
#include <ostream>
#include <random>
#include <stdlib.h>
#include <string>

static const float GUN_OFFSET = -0.0875f;

static const float SCALE = 0.0625f;
static const float SIZEY = SCALE;
static const float SIZEX = (float)((double)SIZEY / (1.0379746835443037974683544303797 * 2.0));
static const float START = 0;

static const float OFFSET_X = 0.0f; // 0.05f
static const float OFFSET_Y = 0.0f; // 0.01f

static const float GUN_FIRE_DURATION = 0.2f;

static const float TEX_MIN_X = -OFFSET_X;
static const float TEX_MAX_X = -1 - OFFSET_X;
static const float TEX_MIN_Y = -OFFSET_Y;
static const float TEX_MAX_Y = 1 - OFFSET_Y;

static const float MOUSE_SENSITIVITY = 0.25f;
static const float MOVE_SPEED = 3.5f;

static const float SHOOT_DISTANCE = 1000.0f;
static const int DAMAGE_MIN = 20;
static const int DAMAGE_MAX = 60;
static const int MAX_HEALTH = 100;

static const Vector3f zeroVector = Vector3f(0, 0, 0);

Mesh* Player::m_mesh = nullptr;
Material* Player::gunMaterial = nullptr;
bool Player::mouseLocked = false;

Player::Player(const Vector3f& pos)
    : m_camera(pos), rd(std::random_device()), rand(rd()), m_health(MAX_HEALTH),
      centerPosition((float)Window::getWidth() / 2, (float)Window::getHeight() / 2),
      movementVector(Vector3f(0, 0, 0))
{
  // system("clear");
  std::cout << "Health : " << m_health << std::endl;

  bool isFiring = false;
  float gunFireTimer = 0.0f;
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
  if (!gunMaterial)
  {
    gunMaterial = new Material(new Texture("PISGB0.png"));
  }

  gunTransform.setPos(Vector3f(7, 0, 7));
}

void Player::damage(int amount)
{
  if (amount > 0)
  {
    Window::playSound("PlayerPain.wav", Window::PLAYER_HEALTH_CHANNEL, 0.7f);
  }
  else
  {
    Window::playSound("Health.wav", Window::PLAYER_HEALTH_CHANNEL, 0.5f);
  }
  m_health -= amount;
  if (m_health > MAX_HEALTH)
  {
    m_health = MAX_HEALTH;
  }
  system("clear");

  std::cout << "Health : " << m_health << std::endl;

  if (m_health <= 0)
  {
    Window::playSound("Player_Dies.wav", Window::PLAYER_DEATH_CHANNEL);
    Game::setIsRunning(false);
    std::cout << "You Just Died ! GAME OVER!" << std::endl;
  }
}

int Player::getDamage()
{
  return std::uniform_int_distribution<>(DAMAGE_MIN, DAMAGE_MAX)(rand);
}

int Player::getHealth()
{
  return m_health;
}

int Player::getMaxHealth()
{
  return MAX_HEALTH;
}

void Player::input()
{
  if (Input::getKey(KEY::KEY_ESCAPE))
  {
    Input::setCursor(true);
    mouseLocked = false;
  }

  if (Input::getKeyDown(KEY::KEY_E))
  {
    Game::getLevel()->openDoors(m_camera.getPos(), true);
  }

  if (Input::getMouseDown(MOUSE::LEFT_MOUSE))
  {
    if (!mouseLocked)
    {
      Input::setMousePosition(centerPosition);
      Input::setCursor(false);
      mouseLocked = true;
    }
    else
    {
      isFiring = true;
      gunFireTimer = GUN_FIRE_DURATION;
      gunMaterial = new Material(new Texture("PISFA0.png"));

      Window::playSound("Pistol.wav", Window::PLAYER_FIRE_CHANNEL, 0.65f);

      Vector2f lineStart(m_camera.getPos().getX(), m_camera.getPos().getZ());
      Vector2f castDirection(m_camera.getForward().getX(), m_camera.getForward().getZ());
      castDirection = castDirection.normalize();
      Vector2f lineEnd = lineStart + castDirection * SHOOT_DISTANCE;

      Game::getLevel()->checkIntersection(lineStart, lineEnd, true);
    }
  }

  movementVector = zeroVector;

  if (Input::getKey(KEY::KEY_W))
    movementVector += m_camera.getForward();
  if (Input::getKey(KEY::KEY_S))
    movementVector -= m_camera.getForward();
  if (Input::getKey(KEY::KEY_A))
    movementVector += m_camera.getLeft();
  if (Input::getKey(KEY::KEY_D))
    movementVector += m_camera.getRight();

  if (mouseLocked)
  {
    Vector2f deltaPos = Input::getMousePosition() - centerPosition;

    bool rotY = deltaPos.x != 0;
    bool rotX = deltaPos.y != 0;

    if (rotY)
      m_camera.rotateY(deltaPos.x * MOUSE_SENSITIVITY);
    if (rotX)
      m_camera.rotateX(deltaPos.y * MOUSE_SENSITIVITY * 0);

    if (rotY || rotX)
      Input::setMousePosition(centerPosition);
  }
}

void Player::update()
{
  float movAmt = (float)(MOVE_SPEED * Time::getDelta());

  if (isFiring)
  {
    gunFireTimer -= Time::getDelta();
    if (gunFireTimer <= 0.0f)
    {
      isFiring = false;
      gunMaterial = new Material(new Texture("PISGB0.png"));
    }
  }

  movementVector.setY(0);
  if (movementVector.length() > 0)
    movementVector = movementVector.normalize();

  Vector3f oldPos = m_camera.getPos();
  Vector3f newPos = oldPos + (movementVector * movAmt);

  Vector3f collisionVector =
      Game::getLevel()->checkCollision(oldPos, newPos, PLAYER_SIZE, PLAYER_SIZE);

  if (collisionVector.getX() != 0 || collisionVector.getZ() != 0)
    movementVector = movementVector.mult(collisionVector);
  if (collisionVector.getX() == 0 && collisionVector.getZ() == 0)
  {
    movementVector.setX(0);
    movementVector.setZ(0);
  }
  if (movementVector.length() > 0)
  {
    Window::playSound("PlayerWalk.wav", Window::PLAYER_WALK_CHANNEL);
    m_camera.move(movementVector, movAmt);
  }
  gunTransform.setPos(m_camera.getPos() + m_camera.getForward().normalize() * 0.105f);
  gunTransform.getPos().setY(gunTransform.getPos().getY() + GUN_OFFSET);

  Vector3f directionToCamera = Transform::getCamera().getPos() - gunTransform.getPos();
  float angleToFaceCamera =
      std::atan2(directionToCamera.getZ(), directionToCamera.getX()) * (180.0f / M_PI);

  if (directionToCamera.getX() < 0)
    angleToFaceCamera += 360;

  gunTransform.getRot().setY(angleToFaceCamera + 90);
}

void Player::render()
{
  Shader* shader = Game::getLevel()->getShader();

  shader->updateUniforms(gunTransform.getTransformation(),
                         gunTransform.getProjectedTransformation(), *gunMaterial);

  m_mesh->draw();
}

Camera& Player::getCamera()
{
  return m_camera;
}

Player::Player(Player&& other) noexcept
    : m_camera(std::move(other.m_camera)), rd(), rand(std::move(other.rand)),
      m_health(other.m_health), centerPosition(std::move(other.centerPosition)),
      movementVector(std::move(other.movementVector))
{
}

Player& Player::operator=(Player&& other) noexcept
{
  if (this != &other)
  {

    m_camera = std::move(other.m_camera);
    m_health = other.m_health;
    centerPosition = std::move(other.centerPosition);
    movementVector = std::move(other.movementVector);

    rd();
    rand = std::move(other.rand);
  }
  return *this;
}
