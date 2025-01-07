
#ifndef MATERIAL_H
#define MATERIAL_H

#include "texture.h"
#include "vector3f.h"

struct Material
{
  Texture* texture;
  Vector3f color;
  float specularIntensity;
  float specularPower;

  Material(Texture* texture = NULL, const Vector3f& color = Vector3f(1, 1, 1),
           float specularIntensity = 2, float specularPower = 32)
  {
    this->texture = texture;
    this->color = color;
    this->specularIntensity = specularIntensity;
    this->specularPower = specularPower;
  }
  Material(const Material& other)
  {
    this->texture = other.texture;
    this->color = other.color;
    this->specularIntensity = other.specularIntensity;
    this->specularPower = other.specularPower;
  }
  Texture* getTexture() const
  {
    return texture;
  }
  void setTexture(Texture* texture)
  {
    this->texture = texture;
  }

  Vector3f getColor() const
  {
    return color;
  }
  void setColor(const Vector3f& color)
  {
    this->color = color;
  }

  float getSpecularPower() const
  {
    return specularPower;
  }
  void setSpecularPower(float value)
  {
    this->specularPower = value;
  }

  float getSpecularIntensity() const
  {
    return specularIntensity;
  }
  void setSpecularIntensity(float value)
  {
    this->specularIntensity = value;
  }
};

#endif
