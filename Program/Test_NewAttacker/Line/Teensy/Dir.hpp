#pragma once

#include <Adafruit_BNO055.h>
#include <Wire.h>

#include "Vec2.hpp"

class Dir{
private:
  Adafruit_BNO055 bno;

  float default_dir;
  float prev_dir;

public:
  float dir;
  Vec2  accel;

  Dir() : bno(55, 0x28, &Wire2), default_dir(0), prev_dir(0), dir(0), accel(0,0) {};

  void begin();
  void calibration();
  void setDefault();
  void read();
};