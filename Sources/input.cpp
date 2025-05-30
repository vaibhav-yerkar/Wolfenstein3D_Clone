#include "input.h"
#include <SDL.h>
#include <SDL_events.h>
#include <SDL_mouse.h>

const static int NUM_KEYS = 512;
const static int NUM_MOUSEBUTTONS = 256;

static SDL_Event e;

static int mouseX = 0;
static int mouseY = 0;

static bool inputs[NUM_KEYS];
static bool downKeys[NUM_KEYS];
static bool upKeys[NUM_KEYS];

static bool mouseInput[NUM_MOUSEBUTTONS];
static bool downMouse[NUM_MOUSEBUTTONS];
static bool upMouse[NUM_MOUSEBUTTONS];

void Input::update()
{
  for (int i = 0; i < NUM_MOUSEBUTTONS; i++)
  {
    downMouse[i] = false;
    upMouse[i] = false;
  }

  for (int i = 0; i < NUM_KEYS; i++)
  {
    downKeys[i] = false;
    upKeys[i] = false;
  }

  while (SDL_PollEvent(&e))
  {
    if (e.type == SDL_QUIT)
      SDLSetIsCloseRequested(true);

    if (e.type == SDL_MOUSEMOTION)
    {
      mouseX = e.motion.x;
      mouseY = e.motion.y;
    }

    if (e.type == SDL_KEYDOWN)
    {
      int value = e.key.keysym.scancode;

      inputs[value] = true;
      downKeys[value] = true;
    }
    if (e.type == SDL_KEYUP)
    {
      int value = e.key.keysym.scancode;

      inputs[value] = false;
      upKeys[value] = true;
    }
    if (e.type == SDL_MOUSEBUTTONDOWN)
    {
      int value = e.button.button;

      mouseInput[value] = true;
      downMouse[value] = true;
    }
    if (e.type == SDL_MOUSEBUTTONUP)
    {
      int value = e.button.button;

      mouseInput[value] = false;
      upMouse[value] = true;
    }
  }
}

bool Input::getKey(int keyCode)
{
  return inputs[keyCode];
}

bool Input::getKeyDown(int keyCode)
{
  return downKeys[keyCode];
}

bool Input::getKeyUp(int keyCode)
{
  return upKeys[keyCode];
}

bool Input::getMouse(int button)
{
  return mouseInput[button];
}

bool Input::getMouseDown(int button)
{
  return downMouse[button];
}

bool Input::getMouseUp(int button)
{
  return upMouse[button];
}

Vector2f Input::getMousePosition()
{
  Vector2f res((float)mouseX, (float)mouseY);
  return res;
}

void Input::setCursor(bool visible)
{
  if (visible)
    SDL_ShowCursor(1);
  else
    SDL_ShowCursor(0);
}

void Input::setMousePosition(Vector2f pos)
{
  SDLSetMousePosition((int)pos.x, (int)pos.y);
}
