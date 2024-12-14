#include <Adafruit_BNO055.h>
#include <Wire.h>
#include "Dir.hpp"

void Dir::begin(){
  if(!bno.begin()){
    Serial.println("bno error!");
    while(true);
  }

  Serial.println("dir setup");
  return;
}

void Dir::calibration(uint8_t* system, uint8_t* gyro, uint8_t* accel, uint8_t* mag){
  bno.getCalibration(system, gyro, accel, mag);
  return;
}

void Dir::setDefault(){
  sensors_event_t d{};
  bno.getEvent(&d, Adafruit_BNO055::VECTOR_EULER);
  default_dir = d.orientation.x;

  return;
}

void Dir::read(){
  sensors_event_t dir_data{};
  bno.getEvent(&dir_data, Adafruit_BNO055::VECTOR_EULER);

  // 角度を更新 [-180~180]
  dir = dir_data.orientation.x-default_dir;
  if(dir<-180) dir += 360;
  if(dir>180)  dir -= 360;

  dir_y = dir_data.orientation.y;
  dir_z = dir_data.orientation.z;

  // 加速度を更新
  bno.getEvent(&dir_data, Adafruit_BNO055::VECTOR_ACCELEROMETER);
  accel.x = dir_data.acceleration.x;
  accel.y = dir_data.acceleration.y;

  prev_dir = dir;
  
  return;
}