#pragma once

#include <Arduino.h>

class Line{
private:
  static constexpr uint8_t NUM = 26;
  const uint8_t PIN[NUM] = {

  }
public:
  bool  line[NUM]; // センサの値
  int   num;               // 反応しているセンサの数

  bool  on;                // 白線上のフラグ
  
  Line() : {}
  // ~Line();

  void begin();
  void read();
};

extern Line line;