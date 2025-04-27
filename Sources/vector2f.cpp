#include "util.h"
#include "vector2f.h"
#include <math.h>

Vector2f& Vector2f::operator+=(const Vector2f& r)
{
  x += r.x;
  y += r.y;

  return *this;
}

Vector2f& Vector2f::operator-=(const Vector2f& r)
{
  x -= r.x;
  y -= r.y;

  return *this;
}

Vector2f& Vector2f::operator*=(float f)
{
  x *= f;
  y *= f;

  return *this;
}

Vector2f& Vector2f::operator/=(float f)
{
  x /= f;
  y /= f;

  return *this;
}

Vector2f Vector2f::operator+(const Vector2f& r) const
{
  Vector2f ret(x + r.x, y + r.y);

  return ret;
}

Vector2f Vector2f::operator-(const Vector2f& r) const
{
  Vector2f ret(x - r.x, y - r.y);
  return ret;
}

Vector2f Vector2f::operator*(float f) const
{
  Vector2f ret(x * f, y * f);
  return ret;
}

Vector2f Vector2f::operator/(float f) const
{
  Vector2f ret(x / f, y / f);
  return ret;
}

Vector2f Vector2f::rotate(float angle)
{
  double rad = ToRadian(angle);
  double cos = cosf(rad);
  double sin = sinf(rad);

  Vector2f ret =
      Vector2f((float)(x * cos - y * sin), (float)(x * sin + y * cos));
  return ret;
}

bool Vector2f::operator==(const Vector2f& r) const
{
  return x == r.x && y == r.y;
}

bool Vector2f::operator!=(const Vector2f& r) const
{
  return !operator==(r);
}

Vector2f Vector2f::mult(const Vector2f& other)
{
  Vector2f ret = Vector2f(x * other.x, y * other.y);
  return ret;
}

float Vector2f::getX() const
{
  return this->x;
}

float Vector2f::getY() const
{
  return this->y;
}

void Vector2f::setX(float value)
{
  this->x = value;
}

void Vector2f::setY(float value)
{
  this->y = value;
}

float Vector2f::length() const
{
  return sqrtf(x * x + y * y);
}

Vector2f& Vector2f::normalize()
{
  const float vectorLength = length();

  x /= vectorLength;
  y /= vectorLength;

  return *this;
}
