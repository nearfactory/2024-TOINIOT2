#pragma once

#include <Arduino.h>

class Ball{
private:
  static constexpr uint8_t NUM = 16;
  const uint8_t PIN[NUM] = {
    A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15
  };

  static constexpr uint16_t DISTANCE_MIN = 7000;
  static constexpr uint16_t DISTANCE_MAX = 16000;

  uint16_t ball[NUM]{0};
public:
  float    dir = 0;
  float    dir_prev = 0;
  uint16_t distance = 0;

  bool     is_exist = true;

  bool     is_hold = false;
  uint32_t hold_time;

  Ball() : dir(0), distance(0), is_exist(true), is_hold(false), hold_time(0) {}

  void begin();
  void read();
};

extern Ball ball;