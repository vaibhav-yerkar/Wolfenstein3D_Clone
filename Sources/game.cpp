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

Game::Game() : m_level("level1.png", "WolfCollection.png")
{
  Transform::setProjection(70.0f, (float)Window::getWidth(),
                           (float)Window::getHeight(), 0.1f, 100.0f);
  Transform::setCamera(m_camera);
}

Game::~Game()
{
}

void Game::input()
{
  Transform::getCamera().input();
}

void Game::update()
{
}

void Game::render()
{
  m_level.render();
}
