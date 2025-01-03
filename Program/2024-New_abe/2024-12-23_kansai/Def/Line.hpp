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

  Vec2  vec1;              // 反応しているセンサのベクトルの左上
  Vec2  vec2;              // 反応しているセンサのベクトルの右下

  static constexpr uint8_t DATA_SIZE = 5; // bit
  static constexpr uint8_t STR_SIZE  = 8;  // byte
  char receive[STR_SIZE] = "";

public:
  bool  line[INNER_NUM+4]; // センサの値
  int   num;               // 反応しているセンサの数

  bool front = false;
  bool left = false;
  bool back = false;
  bool right = false;

  // ラインのベクトル
  Vec2  vec;               
  Vec2  vec_prev;
  float distance;          // ベクトルの長さ

  // 正面に対する白線の角度
  float dir;               
  float dir_prev;

  // 白線上のフラグ
  bool  on;                
  bool  on_prev;

  bool outside = false;
  
  Line() : num(0), vec(0,0), vec_prev(0,0), distance(0), dir(0), on(0) {}
  // ~Line();

  void begin(int rate);
  void read();
  void send(char command  );
};

extern Line line;