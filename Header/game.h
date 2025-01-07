#ifndef GAME_H
#define GAME_H

#include "level.h"

class Game
{
public:
  Game();
  void input();
  void update();
  void render();

  static Level* getLevel();
  static void loadNextLevel();
  static void setIsRunning(bool value);

protected:
private:
  static Level* m_level;
  static bool isRunning;
  static int levelNum;
};

#endif // !GAME_H
