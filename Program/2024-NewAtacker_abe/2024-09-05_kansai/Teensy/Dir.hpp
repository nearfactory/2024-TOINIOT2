#pragma once

#include <Adafruit_BNO055.h>
#include <Wire.h>

Adafruit_BNO055 bno(55, 0x28, &Wire2);

double default_dir = 0.0;
double dir = 0.0;
double prev_dir = 0.0;
double dir_default_display = 0.0;
double accel_sum = 0.0;

inline void dirSetup(){
  if(!bno.begin()){
    Serial.println("bno error!");
    while(true);
  }

  Serial.println("dir setup");
  return;
}

inline void dirCalibration(uint8_t system, uint8_t gyro, uint8_t accel, uint8_t mag){
  bno.getCalibration(&system, &gyro, &accel, &mag);

  return;
}

inline void dirUpdate(){
  prev_dir = dir;
  sensors_event_t orientation{};
  bno.getEvent(&orientation, Adafruit_BNO055::VECTOR_EULER);
  dir = orientation.orientation.x-180;

  sensors_event_t accel{};
  bno.getEvent(&accel, Adafruit_BNO055::VECTOR_ACCELEROMETER);
  accel_sum = accel.acceleration.x + accel.acceleration.y + accel.acceleration.z;

  return;
}