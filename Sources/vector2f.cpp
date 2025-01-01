#include "../Header/vector2f.h"

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

inline bool Vector2f::operator==(const Vector2f& r) const
{
  return x == r.x && y == r.y;
}

inline bool Vector2f::operator!=(const Vector2f& r) const
{
  return !operator==(r);
}

Vector2f Vector2f::operator*(const Vector2f& other)
{
  Vector2f ret = Vector2f(x * other.x, y * other.y);
  return ret;
}

float Vector2f::getX()
{
  return this->x;
}

float Vector2f::getY()
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
