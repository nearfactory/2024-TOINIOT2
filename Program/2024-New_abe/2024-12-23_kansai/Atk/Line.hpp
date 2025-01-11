#include <stdint.h>
#pragma once

#include <Arduino.h>

#include "General.hpp"
#include "Vec2.hpp"

class Line{
private:
  static constexpr uint8_t INNER_NUM = 26;
  static constexpr uint8_t LINE_NUM = INNER_NUM+4;

  uint32_t baudrate = 115200;

  bool prev_on1 = false;
  bool prev_on2 = false;
  bool prev_on3 = false;

  uint32_t right_timer = 0;

  static constexpr uint8_t DATA_SIZE = 5; // bit
  static constexpr uint8_t STR_SIZE = 8;  // byte
  char receive[STR_SIZE] = "";

public:
  bool  line[INNER_NUM+4]; // センサの値
  int   num;               // 反応しているセンサの数
  int   area;              // 連続する部分の数

  bool front = false;
  bool left = false;
  bool back = false;
  bool right = false;

  Vec2  vec;               // ラインのベクトル
  Vec2  vec_prev;

  float distance;          // ロボットの中心点と白線の距離
  float dir;               // ロボットの中心点と白線の角度
  float dir_prev;
  bool  on;                // 白線上のフラグ
  bool  prev_on;

  bool angel   = false;
  bool outside = false;
  
  Line() : num(0), vec(0,0), vec_prev(0,0), distance(0), dir(0), on(0) {}
  // ~Line();

  void begin(int rate);
  void read();
  void send(char command  );
};

extern Line line;