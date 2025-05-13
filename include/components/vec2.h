#pragma once
#include <iostream>

class Vec2{
public:

  Vec2();
  Vec2(float x, float y);
  Vec2(int x, int y);
  Vec2(const std::vector<float>& v);
  void setX(float x);
  float getX();

  void setY(float y);
  float getY();

  float length();

  void normalize();

  Vec2 operator+(const Vec2& v2) const;

  Vec2 operator-(const Vec2& v2) const;

  Vec2 operator*(float scalar);

  Vec2 operator/(float scalar);

  Vec2& operator=(const std::vector<float>& v);

  friend Vec2& operator+=(Vec2& v1, const Vec2& v2);

  friend Vec2& operator-=(Vec2& v1, const Vec2& v2);

  Vec2& operator*=(float scalar);

  Vec2& operator/=(float scalar);

  float dot(const Vec2& other) const;

  float x;
  float y;
};

std::ostream& operator<<(std::ostream& os, const Vec2& vec);

