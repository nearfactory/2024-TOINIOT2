#pragma once

#include <Adafruit_BNO055.h>
#include <Wire.h>

Adafruit_BNO055 bno(55, 0x28, &Wire2);

double dir = 0.0;

inline void dirSetup(){
  if(!bno.begin()){
    Serial.println("bno error!");
    while(true);
  }

  return;
}

inline void dirCalibration(uint8_t system, uint8_t gyro, uint8_t accel, uint8_t mag){
  bno.getCalibration(&system, &gyro, &accel, &mag);

  return;
}

inline void dirUpdate(){
  sensors_event_t orientation{};
  bno.getEvent(&orientation, Adafruit_BNO055::VECTOR_EULER);

  dir = orientation.orientation.x;
  return;
}