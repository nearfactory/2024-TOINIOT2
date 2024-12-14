#pragma once

#include <Arduino.h>

// -100~100[%] の範囲で指定

class Motor{
private:
  // 定数
  static constexpr uint8_t NUM = 4;
  static constexpr uint8_t PWM_TYPE = 2;
  static constexpr uint32_t FREQUENCY = 50000;
  const uint8_t PIN[NUM][PWM_TYPE]{
    {19, 3},
    {17, 2},
    {25, 5},
    {23, 4}
  };
  static constexpr uint8_t EN = 0;
  static constexpr uint8_t PH = 1;
  
  float motor         [NUM] = {0};  // プログラマ用
  float motor_prev    [NUM] = {0};  // 前ループのプログラマ用の値
  float move_dir                  = 0;

  static constexpr uint8_t p_step = 4;
  float p_count[NUM]{0};
  float p_val[NUM]{0};

  static constexpr uint8_t QUEUE_SIZE = 5;   // 移動平均のサンプル数
  float queue[QUEUE_SIZE][NUM]{};  // 出力値のキュー
public:
  float motor_raw     [NUM] = {0};  // モーターに反映するやつ
  // Motor() : {}

  void begin();
  void set(float m1, float m2, float m3, float m4);
  void add(float m1, float m2, float m3, float m4);
  void setDir(float dir, float p_gain);
  void moveDir(float dir, uint8_t power);
  void p();
  void avr();
  void write();
};

extern Motor motor;