#ifndef PLAYER_H
#define PLAYER_H

#include "camera.h"
#include "vector3f.h"

class Player
{
public:
  Player(Vector3f pos);

  void input();
  void update();
  void render();

  Camera& getCamera();

protected:
private:
  Camera m_camera;
  Vector3f movementVector;
};

#endif // !PLAYER_H
