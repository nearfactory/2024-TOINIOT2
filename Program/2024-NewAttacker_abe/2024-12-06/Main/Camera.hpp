#pragma once

#include <vector>

#include <Arduino.h>

#include <Pixy2SPI_SS.h>

// 機体の座標について
// コートが158x219(cm)
// → x:-1~1, y:-1.4~1.4

// 出力値
// 横:320 縦:200

using namespace std;

class Camera{
private:
  struct Block{
    uint16_t sig;
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    int16_t angle;
    uint8_t index;
    uint8_t age;
  };
  // struct HSV{
  //   uint8_t h, s, v;
  // };
  Pixy2SPI_SS pixy;
public:
  // Vec2 pos;
  uint16_t x;
  uint16_t y;
  uint16_t w;
  uint16_t h;

  // Camera() : pos(0,0) {}

  void begin();
  void read();
};

extern Camera camera;