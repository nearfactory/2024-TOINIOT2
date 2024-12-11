#pragma once

#include <Adafruit_BNO055.h>
#include <Wire.h>

#include "Vec2.hpp"

class Dir{
private:
  Adafruit_BNO055 bno;

  float default_dir;
  float prev_dir;

  static constexpr uint8_t QUEUE_SIZE = 10000;
  uint8_t queue_id = 0;
  float queue_y[QUEUE_SIZE]{};
  float queue_z[QUEUE_SIZE]{};
  float avr_y = 0.0f;
  float avr_z = 0.0f;

public:
  float dir;
  float dir_y;
  float dir_z;
  Vec2  accel;

  Dir() : bno(55, 0x28, &Wire2), default_dir(0), prev_dir(0), dir(0), accel(0,0) {};

  void begin();
  void calibration(uint8_t* system, uint8_t* gyro, uint8_t* accel, uint8_t* mag);
  void setDefault();
  void read();
};

extern Dir dir;