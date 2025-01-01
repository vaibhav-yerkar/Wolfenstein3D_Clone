#ifndef GAME_H
#define GAME_H

#include "basicShader.h"
#include "bitmap.h"
#include "camera.h"
#include "level.h"
#include "lighting.h"
#include "mesh.h"
#include "player.h"
#include "shader.h"
#include "transform.h"

class Game
{
public:
  Game();
  ~Game();
  void input();
  void update();
  void render();

  static Game& getInstance();
  Level& getLevel();

protected:
private:
  Player m_player;
  Level m_level;
  Mesh m_mesh;
  Shader* m_shader;
  Vector3f m_color;
  Transform m_transform;
  Camera m_camera;
  Material m_material;
  Texture m_texture;
  PointLight* m_pLights;
  SpotLight* m_sLights;
};

#endif // !GAME_H
