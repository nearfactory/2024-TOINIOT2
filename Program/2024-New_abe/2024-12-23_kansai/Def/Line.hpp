#pragma once

#include <Arduino.h>

class Line{
private:
  static constexpr uint8_t NUM = 22;
  const uint8_t PIN[NUM] = {
    26, 27, 28, 29,
    42, 43, 44, 45, 46, 47,
    36, 37, 38, 39, 40, 41,
    30, 31, 32, 33, 34, 35
  };
public:
  bool line[NUM]; // センサの値
  bool front = false;
  bool left  = false;
  bool back  = false;
  bool right = false;

  float dir = 0;

  int   num;               // 反応しているセンサの数
  bool  on;                // 白線上のフラグ
  bool  on_prev;

  void begin();
  void read();
};

extern Line line;