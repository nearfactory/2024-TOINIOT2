#pragma once

#include <Arduino.h>

class Kicker{
private:
  static constexpr uint8_t PIN = 32;
  uint32_t timer = 0;
  uint32_t interval = 5000;
public:
  uint32_t is_ready = false;
  void begin();
  void kick();
  void write();
};

extern Kicker kicker;