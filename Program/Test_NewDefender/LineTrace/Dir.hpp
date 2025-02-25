#pragma once

#include <Arduino.h>
#define byte uint8_t
// using ::byte;

#include <Adafruit_BNO055.h>
#include <Wire.h>

#include "Vec2.hpp"

class Dir{
private:
  Adafruit_BNO055 bno;

  float default_dir;

public:
  float dir;
  float prev_dir;

  float dir_y;
  float dir_z;
  Vec2  accel;
  bool  is_stole = 0;

  Dir() : bno(55, 0x28, &Wire2), default_dir(0), dir(0), prev_dir(0), accel(0,0) {};

  void begin();
  void calibration(uint8_t* system, uint8_t* gyro, uint8_t* accel, uint8_t* mag);
  void setDefault();
  void read();
};

extern Dir dir;