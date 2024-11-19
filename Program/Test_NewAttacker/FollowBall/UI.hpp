#pragma once

#include <Arduino.h>

class UI{
private:
  static constexpr uint8_t BUTTON_NUM = 4;
  const     uint8_t BUTTON_PIN[BUTTON_NUM] = { 37,31,30,32 };
  
  static constexpr uint8_t TOGGLE_PIN = 36;

  static constexpr uint8_t BZ_PIN = 33;
  float  bz = 0;
  
  bool button[BUTTON_NUM] = {false};
  bool previous_button[BUTTON_NUM] = {false};
public:
  void begin();
  void read();
  bool buttonUp(uint8_t id);
  void buzzer(float frequency);
};

extern UI ui;