#pragma once

#include <Adafruit_BNO055.h>
#include <Wire.h>

Adafruit_BNO055 bno(55, 0x28, &Wire2);

double default_dir = 0.0;
double dir = 0.0;           // -180 - 180
double prev_dir = 0.0;
double dir_default_display = 0.0;
double accel_x = 0.0;
double accel_y = 0.0;
double accel_z = 0.0;
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
  dir = orientation.orientation.x-default_dir;
  if(dir<-180) dir += 360;
  if(dir>180)  dir -= 360;
  // dir = 

  sensors_event_t accel{};
  bno.getEvent(&accel, Adafruit_BNO055::VECTOR_ACCELEROMETER);
  accel_x = accel.acceleration.x;
  accel_y = accel.acceleration.y;
  accel_z = accel.acceleration.z;

  accel_sum = accel_x+accel_y+accel_z;

  return;
}