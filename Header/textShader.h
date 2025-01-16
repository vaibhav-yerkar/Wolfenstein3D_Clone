#ifndef TEXTSHADER_H
#define TEXTSHADER_H

#include <GL/glew.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class TextShader
{
public:
  unsigned int ID;
  TextShader(const std::string& vertexName, const std::string& fragmentName);

  void use();
  void setBool(const std::string& name, bool value) const;
  void setInt(const std::string& name, int value) const;
  void setFloat(const std::string& name, float value) const;
  void setVec3(const std::string& name, float x, float y, float z) const;
  void setMat4(const std::string& name, const float* mat) const;

protected:
private:
};

#endif // TEXTSHADER_H
