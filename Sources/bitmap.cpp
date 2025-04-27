#include "bitmap.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL_surface.h>
#include <iostream>
#include <sys/types.h>

Bitmap::Bitmap(int width, int height)
{
  m_width = width;
  m_height = height;
  m_pixels.resize(m_width * m_height);
}

Bitmap::Bitmap(const std::string& fileName)
{
  SDL_Surface* image =
      IMG_Load((std::string(RESOURCE_PATH) + "/bitmaps/" + fileName).c_str());
  if (!image)
  {
    std::cerr << "Failed to load image : " << fileName << "\n"
              << IMG_GetError() << std::endl;
  }
  m_width = image->w;
  m_height = image->h;
  m_pixels.resize(m_width * m_height);

  int* pixels = static_cast<int*>(image->pixels);
  for (int i = 0; i < m_width * m_height; i++)
  {
    m_pixels[i] = pixels[i];
  }
  SDL_FreeSurface(image);
}

Bitmap::Bitmap(const Bitmap& other)
{
  m_width = other.m_width;
  m_height = other.m_height;
  m_pixels = other.m_pixels;
}

Bitmap& Bitmap::flipX()
{
  std::vector<int> temp = m_pixels;

  for (int i = 0; i < m_width; ++i)
  {
    for (int j = 0; j < m_height; ++j)
    {
      temp[i + j * m_width] = m_pixels[(m_width - i - 1) + j * m_width];
    }
  }

  m_pixels = temp;
  return *this;
}

Bitmap& Bitmap::flipY()
{
  std::vector<int> temp = m_pixels;

  for (int i = 0; i < m_width; ++i)
  {
    for (int j = 0; j < m_height; ++j)
    {
      temp[i + j * m_width] = m_pixels[i + (m_height - j - 1) * m_width];
    }
  }

  m_pixels = temp;
  return *this;
}

Bitmap& Bitmap::operator=(const Bitmap& other)
{
  if (this == &other)
    return *this;

  m_width = other.m_width;
  m_height = other.m_height;
  m_pixels = other.m_pixels;

  return *this;
}

Bitmap& Bitmap::operator=(Bitmap* other)
{
  if (this == other)
    return *this;
  if (other)
  {
    m_width = other->m_width;
    m_height = other->m_height;
    m_pixels = other->m_pixels;
  }

  return *this;
}

int Bitmap::getWidth()
{
  return m_width;
}

int Bitmap::getHeight()
{
  return m_height;
}

const std::vector<int> Bitmap::getPixels() const
{
  return m_pixels;
}

int Bitmap::getPixel(int x, int y) const
{
  return m_pixels[x + y * m_width];
}

void Bitmap::setPixel(int x, int y, int value)
{
  m_pixels[x + y * m_width] = value;
}
