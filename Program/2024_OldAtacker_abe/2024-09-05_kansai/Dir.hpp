#pragma once

#include <Adafruit_BNO055.h>
#include <Wire.h>

Adafruit_BNO055 bno(55, 0x28, &Wire);

double default_dir = 0.0;
double dir = 0.0;           // -180 - 180
double prev_dir = 0.0;

double accel_x = 0.0;
double accel_y = 0.0;
double accel_z = 0.0;
double accel_sum = 0.0;

double atk_x = 0.0;
double atk_y = 0.0;
// 自陣側左下を(0,0), 敵陣側右上を(1,1)

inline void dirSetup(){
  Wire.begin();
  
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
  sensors_event_t dir_data{};
  bno.getEvent(&dir_data, Adafruit_BNO055::VECTOR_EULER);
  bno.getEvent(&dir_data, Adafruit_BNO055::VECTOR_ACCELEROMETER);
  // bno.getEvent(&dir_data, Adafruit_BNO055::)

  // 角度を更新 [-180~180]
  dir = dir_data.orientation.x-default_dir;
  if(dir<-180) dir += 360;
  if(dir>180)  dir -= 360;

  // 加速度を更新
  accel_x = dir_data.acceleration.x;
  accel_y = dir_data.acceleration.y;
  accel_z = dir_data.acceleration.z;
  accel_sum = accel_x+accel_y+accel_z;

  // 位置を計算

  return;
}