#include <cmath>
#include "Vec2.hpp"

float Vec2::len(){
  return std::sqrt(x*x + y*y);
}