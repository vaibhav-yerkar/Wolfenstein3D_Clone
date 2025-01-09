#include "../Header/sdl_requests.h"
#include "SDL_render.h"

static SDL_Window* window;
static SDL_GLContext glContext;
static bool isCloseRequested = false;
static SDL_Renderer* renderer;

bool SDLGetIsCloseRequested()
{
  return isCloseRequested;
}

void SDLSetIsCloseRequested(bool value)
{
  isCloseRequested = value;
}

void SDLCreateWindow(const char* title, int x, int y, int width, int height, bool fullscreen)
{
  int mode = 0;

  if (fullscreen)
    mode = SDL_WINDOW_FULLSCREEN;

  window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
                            SDL_WINDOW_OPENGL | mode);
  glContext = SDL_GL_CreateContext(window);

  // SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
}

void SDLSetWindowFullscreen(bool value)
{
  int mode = 0;
  if (value)
    mode = SDL_WINDOW_FULLSCREEN;
  else
    mode = 0;

  SDL_SetWindowFullscreen(window, mode);
}

void SDLSwapBuffers()
{
  SDL_GL_SwapWindow(window);
}

void SDLDestroyWindow()
{
  SDL_GL_DeleteContext(glContext);
  SDL_DestroyWindow(window);
}

void SDLDestroyRenderer()
{
  SDL_DestroyRenderer(renderer);
}

void SDLSetMousePosition(int x, int y)
{
  SDL_WarpMouseInWindow(window, x, y);
}

SDL_Renderer* SDLCreateRenderer(int index, uint flag)
{
  renderer = SDL_CreateRenderer(window, index, flag);
  return renderer;
}
