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

void Dir::calibration(){
  uint8_t system = 0, gyro = 0, accel = 0, mag = 0;
  // system = 3; gyro = 3; mag = 3;
  
  // キャリブレーション完了まで繰り返し
  while(system<3 || gyro<3 || mag<3){
    Serial.println("calibration...");

    // キャリブレーション状況を取得・ディスプレイバッファへ書き込み
    bno.getCalibration(&system, &gyro, &accel, &mag);
  }
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

  // 加速度を更新
  bno.getEvent(&dir_data, Adafruit_BNO055::VECTOR_ACCELEROMETER);
  accel.x = dir_data.acceleration.x;
  accel.y = dir_data.acceleration.y;

  return;
}