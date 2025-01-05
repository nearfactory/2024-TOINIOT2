#pragma once

#include <Arduino.h>

class Sub{
  static constexpr uint8_t STR_SIZE = 4;

  uint32_t kicked_timer = 0;  
public:
  uint8_t ball01k = 0;
  uint8_t ball02k = 0;
  uint8_t volume  = 0;

  bool is_hold = false;

  void begin();
  void read();
  void kick();
};

extern Sub sub;