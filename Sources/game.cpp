#include "game.h"
#include "window.h"

#include <math.h>
double temp = 0.0f;

Level* Game::m_level = nullptr;
bool Game::isRunning = false;
int Game::levelNum = 0;

Game::Game()
{
  loadNextLevel();
}

void Game::input()
{
  if (m_level)
    m_level->input();
}

void Game::update()
{
  if (isRunning && m_level)
    m_level->update();
}

void Game::render()
{
  if (isRunning && m_level)
    m_level->render();
}

void Game::loadNextLevel()
{
  levelNum++;
  std::string levelFile = "level" + std::to_string(levelNum) + ".png";
  std::string collectionFile = "WolfCollection.png";

  delete m_level;
  m_level = new Level(levelFile, collectionFile);

  Transform::setProjection(70.0f, (float)Window::getWidth(),
                           (float)Window::getHeight(), 0.1f, 100.0f);
  Transform::setCamera(m_level->getPlayer()->getCamera());

  isRunning = true;
}

Level* Game::getLevel()
{
  return m_level;
}

void Game::setIsRunning(bool value)
{
  isRunning = value;
}
