#include <cmath>
#include "Vec2.hpp"

float Vec2::len(){
  return std::sqrt(x*x + y*y);
}

void Vec2::clear(){
  x = 0.0f;
  y = 0.0f;
  return;
}