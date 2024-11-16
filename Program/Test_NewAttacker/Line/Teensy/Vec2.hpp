#pragma once

class Vec2{
public:
  float x=0.0f, y=0.0f;
  Vec2(float vec_x, float vec_y) : x(vec_x), y(vec_y){}

  void clear();
  float len();
};