#include "../Header/player.h"
#include "../Header/custom_time.h"
#include "../Header/game.h"
#include "../Header/input.h"
#include "../Header/level.h"
#include "../Header/vector2f.h"
#include "../Header/window.h"

#include <iostream>

static const float MOUSE_SENSITIVITY = 0.5f;
static const float MOVE_SPEED = 4.0f;
static const float PLAYER_SIZE = 0.18f;

static const Vector3f zeroVector = Vector3f(0, 0, 0);

static bool mouseLocked = false;

Player::Player(Vector3f pos)
{
  m_camera = Camera(pos);
}

void Player::input()
{
  Vector2f centerPosition =
      Vector2f((float)Window::getWidth() / 2.0f, (float)Window::getHeight() / 2.0f);

  if (Input::getKey(KEY::KEY_ESCAPE))
  {
    Input::setCursor(true);
    mouseLocked = false;
  }

  if (!mouseLocked && Input::getMouseDown(MOUSE::LEFT_MOUSE))
  {
    Input::setMousePosition(centerPosition);
    Input::setCursor(false);
    mouseLocked = true;
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
      m_camera.rotateX(deltaPos.y * MOUSE_SENSITIVITY);

    if (rotY || rotX)
      Input::setMousePosition(centerPosition);
  }
}

void Player::update(Level& m_level)
{
  float movAmt = (float)(MOVE_SPEED * Time::getDelta());

  movementVector.setY(0);
  if (movementVector.length() > 0)
    movementVector = movementVector.normalize();

  Vector3f oldPos = m_camera.getPos();
  Vector3f newPos = oldPos + (movementVector * movAmt);

  Vector3f collisionVector = m_level.checkCollision(oldPos, newPos, PLAYER_SIZE, PLAYER_SIZE);
  if (collisionVector.getX() != 0 || collisionVector.getZ() != 0)
    movementVector = movementVector.mult(collisionVector);

  if (movementVector.length() > 0)
    m_camera.move(movementVector, movAmt);
}

void Player::render()
{
}

Camera& Player::getCamera()
{
  return m_camera;
}
