#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "vector2f.h"
#include "vector3f.h"
#include <string>

class Window
{
public:
  static constexpr int PLAYER_FIRE_CHANNEL = 0;
  static constexpr int PLAYER_WALK_CHANNEL = 1;
  static constexpr int PLAYER_HEALTH_CHANNEL = 2;
  static constexpr int PLAYER_DEATH_CHANNEL = 3;
  static constexpr int MONSTER_FIRE_CHANNEL = 4;
  static constexpr int MONSTER_HEALTH_CHANNEL = 5;
  static constexpr int DOOR_OPEN_CHANNEL = 6;
  static constexpr int DOOR_CLOSE_CHANNEL = 7;

  static void create(int width, int height, const std::string& title);
  static void render();
  static void dispose();

  static bool isCloseRequested();
  static int getWidth();
  static int getHeight();
  static const std::string& getTitle();

  static void setFullScreen(bool value);

  static void playSound(const std::string& fileName, int channel = -1, float volume = 1.0f);

protected:
private:
  static int m_width;
  static int m_height;
  static std::string m_title;
};

#endif
