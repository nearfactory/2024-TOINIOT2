#pragma once

class Line{
private:
  static constexpr uint8_t LINE_INNER_NUM = 26;
  static constexpr uint8_t LINE_NUM = LINE_INNNER_NUM+4;

  uint32_t baudrate = 115200;
  char receive[STR_SIZE] = "";

  static constexpr uint8_t DATA_SIZE = 5;
  static constexpr uint8_t STR_SIZE = 8;
public:
  bool  data[INNER_NUM+4]{0}; // センサの値
  int   num = 0;              // 反応しているセンサの数
  float dir = 0.0f;           // ロボットの中心点と白線の角度
  bool  on = false;           // 白線上のフラグ
  
  Line(){};
  ~Line();

  void Begin();
  void Read();
}