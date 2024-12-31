#pragma once

#include <Arduino.h>

#include "Vec2.hpp"

// 機体の座標について
// コートが158x219(cm)
// → x:-1~1, y:-1.4~1.4

class Camera{
private:
  struct HSV{
    uint8_t h, s, v;
  };

  uint8_t blue_min;
  uint8_t blue_max;
  uint8_t yellow_mix;
  uint8_t yellow_max;

public:
  Vec2 pos;
  float def_dir = 0;

  Camera() : pos(0,0) {}

  void read();
};

extern Camera camera;