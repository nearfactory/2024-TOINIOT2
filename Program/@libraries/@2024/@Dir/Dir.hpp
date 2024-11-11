#pragma once

#include <Adafruit_BNO055.h>
#include <Wire.h>

// 角度: -180 - 180

class Dir
{
private:
  Adafruit_BNO055 bno(55, 0x28, &Wire2);
  double default_dir = 0.0;

public:
  double dir = 0.0;

  double accel_x = 0.0;
  double accel_y = 0.0;
  double accel_z = 0.0;
  double accel_sum = 0.0;

  Dir();
  ~Dir();

  void setup();
  void get();
};

inline void Dir::setup(){
  if(!bno.begin()){
    Serial.println("bno error!");
    while(true);
  }

  Serial.println("dir ok");
  return;
}

inline double DIr::get(){
  sensors_event_t dir_data{};
  bno.getEvent(&dir_data, Adafruit_BNO055::VECTOR_EULER);

  // 角度を更新 [-180~180]
  dir = dir_data.orientation.x-default_dir;
  if(dir<-180) dir += 360;
  if(dir>180)  dir -= 360;

  // 加速度を更新
  bno.getEvent(&dir_data, Adafruit_BNO055::VECTOR_ACCELEROMETER);
  accel_x = dir_data.acceleration.x;
  accel_y = dir_data.acceleration.y;
  accel_z = dir_data.acceleration.z;
  accel_sum = accel_x+accel_y+accel_z;

  return dir;
}