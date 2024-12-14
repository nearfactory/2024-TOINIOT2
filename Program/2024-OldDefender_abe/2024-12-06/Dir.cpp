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
  prev_dir = dir;
  sensors_event_t dir_data{};
  bno.getEvent(&dir_data, Adafruit_BNO055::VECTOR_EULER);

  // 角度を更新 [-180~180]
  dir = dir_data.orientation.x-default_dir;
  if(dir<-180) dir += 360;
  if(dir>180)  dir -= 360;

  // dir_y = dir_data.orientation.y;
  // dir_z = dir_data.orientation.z;



  queue_y[queue_id] = dir_data.orientation.y;
  queue_z[queue_id] = 180.0f - abs(dir_data.orientation.z);
  avr_y = 0;
  avr_z = 0;
  for(auto y:queue_y) avr_y += y;
  for(auto z:queue_z) avr_z += z;
  avr_y /= (float)QUEUE_SIZE;
  avr_z /= (float)QUEUE_SIZE;
  dir_y = dir_data.orientation.y - avr_y;
  dir_z = 180.0f - dir_data.orientation.z - avr_z;
  if(dir_z > 180.0f) dir_z = dir_z - 360.0f;
  float threshold = 1.0f;
  // if(abs(avr_y - dir_y) < 1.0f && abs(avr_z - dir_z) < 1.0f && abs(dir_y) < 4.0f && 180.0f - abs(dir_z) < 4.0f)
  // if(abs(dir_y) < 4.0f && 180.0f - abs(dir_z) < 4.0f)
    queue_id = (queue_id+1)%QUEUE_SIZE;

  // dir_y = dir_data.orientation.y;
  // dir_z = dir_data.orientation.z;

  // dir_y = avr_y;
  // dir_z = avr_z;

  // dir_y = 0;
  // dir_z = 0;



  // Serial.printf("x:%f y:%f z:%f\n", dir_data.orientation.x, dir_data.orientation.y, dir_data.orientation.z);

  // 加速度を更新
  bno.getEvent(&dir_data, Adafruit_BNO055::VECTOR_ACCELEROMETER);
  accel.x = dir_data.acceleration.x;
  accel.y = dir_data.acceleration.y;

  return;
}