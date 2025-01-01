#ifndef BITMAP_H
#define BITMAP_H

#include <string>
#include <vector>

class Bitmap
{
public:
  Bitmap(const std::string& fileName);
  Bitmap(int width, int height);

  Bitmap& flipX();
  Bitmap& flipY();

  int getWidht();
  int getHeight();
  const std::vector<int> getPixels() const;
  int getPixel(int x, int y) const;

  void setPixel(int x, int y, int value);

  Bitmap& operator=(const Bitmap& other);
  Bitmap& operator=(Bitmap* other);

  Bitmap(const Bitmap& other);

protected:
private:
  int m_width;
  int m_height;
  std::vector<int> m_pixels;
};

#endif // !BITMAP_H
