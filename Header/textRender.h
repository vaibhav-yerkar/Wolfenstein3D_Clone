#ifndef TEXTRENDER_H
#define TEXTRENDER_H

#include <iostream>
#include <map>
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "textShader.h"
#include "vector2f.h"
#include "vector3f.h"

struct Character
{
  unsigned int TextureID; // ID handle of the glyph texture
  glm::ivec2 Size;        // Size of glyph
  glm::ivec2 Bearing;     // Offset from baseline to left/top of glyph
  unsigned int Advance;   // Horizontal offset to advance to next glyph
};

class TextRender
{
public:
  static TextRender* getInstance();
  bool initialize(const std::string& fontName, int defaultFontSize);

  void renderText(const std::string& text, const Vector2f& pos, const Vector3f& color,
                  int fontSize = 0);

  void cleanup();

protected:
private:
  std::map<GLchar, Character> Characters;
  unsigned int VAO, VBO;

  FT_Library ft;
  FT_Face face;
  int defaultSize;
};

#endif // !TEXTRENDER_H
