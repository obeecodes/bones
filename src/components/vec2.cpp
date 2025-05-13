#include "../../include/components/vec2.h"
#include <math.h>
#include <vector>

Vec2::Vec2(){
  this->x = 0;
  this->y = 0;
}

Vec2::Vec2(float x, float y){
  this->x = x;
  this->y = y;
}


Vec2::Vec2(int x, int y){
  this->x = static_cast<float>(x);
  this->y = static_cast<float>(y);
}

Vec2::Vec2(const std::vector<float>& v){
  if (v.size() != 2) {
    throw std::invalid_argument("Vector size must be 2");
  }

  this->x = v[0];
  this->y = v[1];
}

void Vec2::setX(float x){
  this->x = x;
}

float Vec2::getX(){
  return this->x;
}

void Vec2::setY(float y){
  this->y = y;
}

float Vec2::getY(){
  return this->y;
}

float Vec2::length(){
  return sqrt(this->x * this->x + this->y * this->y);
}

void Vec2::normalize(){
  float vecLength = length();

  if(vecLength > 0){
    (*this) *= 1 / vecLength;
  }

}

float Vec2::dot(const Vec2& v) const {
  return this->x * v.x + this->y * v.y;
}

Vec2 Vec2::operator+(const Vec2& v2) const{
  return Vec2(this->x + v2.x, this->y + v2.y);
}

Vec2 Vec2::operator-(const Vec2& v2) const{
  return Vec2(this->x - v2.x, this->y - v2.y);
}

Vec2 Vec2::operator*(float scalar){
  return Vec2(this->x * scalar, this->y * scalar);
}

Vec2 Vec2::operator/(float scalar){
  return Vec2(this->x / scalar, this->y / scalar);
}

Vec2& operator+=(Vec2& v1, const Vec2& v2){
  v1.x += v2.x;
  v1.y += v2.y;

  return v1;
}

Vec2& operator-=(Vec2& v1, const Vec2& v2){
  v1.x -= v2.x;
  v1.y -= v2.y;

  return v1;
}

Vec2& Vec2::operator*=(float scalar){
  this->x *= scalar;
  this->y *= scalar;

  return *this;
}

Vec2& Vec2::operator/=(float scalar){
  this->x /= scalar;
  this->y /= scalar;

  return *this;
}

Vec2& Vec2::operator=(const std::vector<float>& v) {
  if (v.size() != 2) {
    throw std::invalid_argument("Vector size must be 2");
  }

  this->x = v[0];
  this->y = v[1];
  return *this;
}

std::ostream& operator<<(std::ostream& os, const Vec2& vec){
  os << "( "<< vec.x << ", " << vec.y << " )";
  return os; 
}
