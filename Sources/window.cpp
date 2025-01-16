#include "../Header/window.h"
#include "../Header/sdl_requests.h"

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <iostream>

int Window::m_width = 0;
int Window::m_height = 0;
std::string Window::m_title = "Title";

void Window::create(int width, int height, const std::string& title)
{
  Window::m_width = width;
  Window::m_height = height;
  Window::m_title = title;

  SDL_Init(SDL_INIT_VIDEO);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  SDLCreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
                  false);

  GLenum res = glewInit();
  if (res != GLEW_OK)
  {
    fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
  }

  if (SDL_Init(SDL_INIT_AUDIO) < 0)
  {
    std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
  }

  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
  {
    std::cerr << "Failed to initialize SDL_mixer: " << Mix_GetError() << std::endl;
    SDL_Quit();
  }
  Mix_AllocateChannels(16);
}

void Window::render()
{
  SDLSwapBuffers();
}

void Window::dispose()
{
  Mix_Quit();
  SDLDestroyWindow();
  SDL_Quit();
}

bool Window::isCloseRequested()
{
  return SDLGetIsCloseRequested();
}

void Window::setFullScreen(bool value)
{
  SDLSetWindowFullscreen(value);
}

int Window::getWidth()
{
  return m_width;
}

int Window::getHeight()
{
  return m_height;
}

const std::string& Window::getTitle()
{
  return m_title;
}

void Window::playSound(const std::string& fileName, int channel, float volume)
{
  std::string filePath = std::string(RESOURCE_PATH) + "/audio/" + fileName;
  Mix_Chunk* sound = Mix_LoadWAV(filePath.c_str());

  if (!sound)
  {
    std::cerr << "Failed to load sound: " << Mix_GetError() << std::endl;
    Mix_CloseAudio();
    return;
  }
  Mix_MasterVolume(volume * MIX_MAX_VOLUME);
  if ((channel == PLAYER_FIRE_CHANNEL || channel == MONSTER_FIRE_CHANNEL))
  {
    Mix_PlayChannel(channel, sound, 0);
  }
  else if (Mix_Playing(channel) > 0)
  {
    return;
  }
  if (Mix_PlayChannel(channel, sound, 0) == -1)
  {
    std::cerr << "Failed to play sound: " << Mix_GetError() << std::endl;
    Mix_FreeChunk(sound);
    return;
  }
}
