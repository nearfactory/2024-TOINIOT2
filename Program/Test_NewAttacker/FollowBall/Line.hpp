#pragma once

#include <Arduino.h>
#include "Vec2.hpp"

class Line{
private:
  static constexpr uint8_t INNER_NUM = 26;
  static constexpr uint8_t LINE_NUM = INNER_NUM+4;

  uint32_t baudrate = 115200;

  static constexpr uint8_t DATA_SIZE = 5; // bit
  static constexpr uint8_t STR_SIZE = 8;  // byte
  char receive[STR_SIZE] = "";
public:
  bool  line[INNER_NUM+4]; // センサの値
  int   num;               // 反応しているセンサの数
  Vec2  vec;               // 反応しているセンサのベクトルの合計

  float distance;          // ロボットの中心点と白線の距離
  float dir;               // ロボットの中心点と白線の角度
  float dir_prev;
  bool  on;                // 白線上のフラグ
  
  Line() : num(0), vec(0,0), distance(0), dir(0), on(0) {}
  // ~Line();

  void begin(int rate);
  void read();
};

extern Line line;