#include "../Header/game.h"
#include "../Header/basicShader.h"
#include "../Header/bitmap.h"
#include "../Header/custom_time.h"
#include "../Header/input.h"
#include "../Header/phongShader.h"
#include "../Header/texture.h"
#include "../Header/util.h"
#include "../Header/window.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <math.h>
#include <vector>
double temp = 0.0f;

Game::Game() : m_level("level1.png", "WolfCollection.png"), m_player(Vector3f(6, 0.4375f, 6))
{
  Transform::setProjection(70.0f, (float)Window::getWidth(), (float)Window::getHeight(), 0.1f,
                           100.0f);
  Transform::setCamera(m_player.getCamera());
}

Game::~Game()
{
}

void Game::input()
{
  m_level.input();
  m_player.input();
}

void Game::update()
{
  m_level.update();
  m_player.update(m_level);
}

void Game::render()
{
  m_level.render();
  m_player.render();
}

Game& Game::getInstance()
{
  static Game instance;
  return instance;
}

Level& Game::getLevel()
{
  return m_level;
}
