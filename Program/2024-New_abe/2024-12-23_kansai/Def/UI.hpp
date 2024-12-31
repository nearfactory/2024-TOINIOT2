#pragma once

#include <Arduino.h>

class UI{
private:
  static constexpr uint8_t BUTTON_NUM = 3;
  const     uint8_t BUTTON_PIN[BUTTON_NUM] = { 7,8,9 };
  
  static constexpr uint8_t TOGGLE_PIN = 10;

  static constexpr uint8_t BZ_PIN = 6;
  float  bz = 0;
  
  bool button[BUTTON_NUM]{};
  bool previous_button[BUTTON_NUM]{};
public:
  const uint8_t TOGGLE = 10;

  void begin();
  void read();
  bool buttonUp(uint8_t id);
  void buzzer(float frequency);
};

extern UI ui;