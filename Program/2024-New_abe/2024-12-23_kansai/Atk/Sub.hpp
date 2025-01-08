#pragma once

#include <Arduino.h>

class Sub{
  static constexpr uint8_t STR_SIZE = 4;

  static constexpr uint8_t THRESHOLD = 128;

  static constexpr uint16_t INTERVAL = 10000;
  uint32_t kicked_timer = 0;  
public:
  uint8_t ball01k = 0;
  uint8_t ball02k = 0;
  uint8_t volume  = 0;

  uint8_t brightness = 0;

  bool is_hold = false;
  bool ready   = false;

  void begin();
  void read();
  void kick();
};

extern Sub sub;