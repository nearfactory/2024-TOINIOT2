#pragma once

#include <Arduino.h>

class Kicker{
private:
  static constexpr uint8_t KICKER_PIN = 32;
  uint32_t kicked_timer = 0;
public:
  void begin();
  void kick();
  void write();
};

extern Kicker kicker;