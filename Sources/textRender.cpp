#include "../Header/textRender.h"
#include <iostream>

TextRender* TextRender::getInstance()
{
  static TextRender instance;
  return &instance;
}

bool TextRender::initialize(const std::string& fontName, int defaultFontSize)
{
  this->defaultSize = defaultFontSize;

  if (FT_Init_FreeType(&ft))
  {
    std::cerr << "Could not initialize FreeType library" << std::endl;
    return false;
  }

  std::string fontPath = std::string(RESOURCE_PATH) + "/fonts/" + fontName;

  if (FT_New_Face(ft, fontPath.c_str(), 0, &face))
  {
    std::cerr << "Failed to load font : " << fontPath << std::endl;
    return false;
  }

  FT_Set_Pixel_Sizes(face, 0, defaultSize);
  return true;
}

void TextRender::renderText(const std::string& text, const Vector2f& pos, const Vector3f& color,
                            int fontSize)
{
  if (!face)
  {
    std::cerr << "Font not loaded, call initialize() first." << std::endl;
    return;
  }

  if (fontSize > 0 && fontSize != defaultSize)
  {
    FT_Set_Pixel_Sizes(face, 0, fontSize);
  }

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  //  TextShader* shader = TextShader::getInstance();
  //  shader->bind();
  //  shader->updateUniforms(Matrix4f::createTranslationTransform(pos.x, pos.y, 0), color);

  float x = pos.x;
  float y = pos.y;

  GLuint VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

  for (char c : text)
  {
    if (FT_Load_Char(face, c, FT_LOAD_RENDER))
    {
      std::cerr << "Failed to load glyph for character: " << c << std::endl;
      continue;
    }

    FT_GlyphSlot g = face->glyph;

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, g->bitmap.width, g->bitmap.rows, 0, GL_RED,
                 GL_UNSIGNED_BYTE, g->bitmap.buffer);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    float xpos = x + g->bitmap_left;
    float ypos = y - (g->bitmap.rows - g->bitmap_top);

    float w = g->bitmap.width;
    float h = g->bitmap.rows;

    float vertices[6][4] = {{xpos, ypos + h, 0.0f, 0.0f},    {xpos, ypos, 0.0f, 1.0f},
                            {xpos + w, ypos, 1.0f, 1.0f},

                            {xpos, ypos + h, 0.0f, 0.0f},    {xpos + w, ypos, 1.0f, 1.0f},
                            {xpos + w, ypos + h, 1.0f, 0.0f}};

    glBindTexture(GL_TEXTURE_2D, texture);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDeleteTextures(1, &texture);

    x += (g->advance.x >> 6); // Advance cursor to the next glyph
    y += (g->advance.y >> 6);
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glDeleteBuffers(1, &VBO);
  glDeleteVertexArrays(1, &VAO);

  glBindTexture(GL_TEXTURE_2D, 0);

  // Reset font size to default
  if (fontSize > 0 && fontSize != defaultSize)
  {
    FT_Set_Pixel_Sizes(face, 0, defaultSize);
  }
}

void TextRender::cleanup()
{
  if (face)
  {
    FT_Done_Face(face);
    face = nullptr;
  }

  if (ft)
  {
    FT_Done_FreeType(ft);
    ft = nullptr;
  }
}
