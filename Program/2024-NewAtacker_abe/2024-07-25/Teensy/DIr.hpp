#pragma once

#include <Adafruit_BNO055.h>
#include <Wire.h>

Adafruit_BNO055 bno(55, 0x28, &Wire2);

inline void dirSetup(){
  if(!bno.begin()){
    Serial.println("bno error!");
    while(true);
  }

  return;
}

inline double dirUpdate(){
  sensors_event_t orientation{};
  bno.getEvent(&orientation, Adafruit_BNO055::VECTOR_EULER);

  return orientation.orientation.x;
}