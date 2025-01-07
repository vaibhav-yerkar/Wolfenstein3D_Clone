#ifndef VECTOR2F_H
#define VECTOR2F_H

class Vector2f
{
public:
  float x;
  float y;

  Vector2f(float x = 0.0f, float y = 0.0f)
  {
    this->x = x;
    this->y = y;
  }

  float getX() const;
  float getY() const;

  float length() const;

  Vector2f rotate(float angle);

  void setX(float value);
  void setY(float value);

  Vector2f& normalize();

  Vector2f& operator+=(const Vector2f& r);
  Vector2f& operator-=(const Vector2f& r);
  Vector2f& operator*=(float f);
  Vector2f& operator/=(float f);

  Vector2f operator+(const Vector2f& r) const;
  Vector2f operator-(const Vector2f& r) const;
  Vector2f operator*(float f) const;
  Vector2f operator/(float f) const;

  Vector2f mult(const Vector2f& other);

  bool operator==(const Vector2f& r) const;
  bool operator!=(const Vector2f& r) const;

protected:
private:
};

#endif
