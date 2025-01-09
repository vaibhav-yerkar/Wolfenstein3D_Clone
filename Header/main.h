#ifndef MAINCOMPONENT_H
#define MAINCOMPONENT_H

#include "custom_time.h"
#include "game.h"
#include "input.h"
#include "renderUtil.h"
#include "util.h"
#include "window.h"

class Main
{
public:
  Main();
  ~Main();

  void start();
  void stop();

protected:
private:
  bool isRunning;
  Game game;

  void run();
};

#endif // MAINCOMPONENT_H
