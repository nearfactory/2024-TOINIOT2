#include <Arduino.h>
#include <Adafruit_BNO055.h>
#include <Wire.h>

#include "Motor.hpp"

Adafruit_BNO055 bno(55, 0x28, &Wire2);
double default_dir = 0;

void setup(){
  Serial.begin(9600);
  Serial.println("TOINIOT2 SetDir Test 2024-09-04");

  Wire2.begin();

  if(!bno.begin()){
    while(true);
  }

  Serial.println("setup()");

  delay(500);
  sensors_event_t orientation;
  bno.getEvent(&orientation, Adafruit_BNO055::VECTOR_EULER);
  default_dir = orientation.orientation.x;
}

void loop(){
  sensors_event_t orientation;
  bno.getEvent(&orientation, Adafruit_BNO055::VECTOR_EULER);
  double dir = orientation.orientation.x - default_dir;
  static double prev_dir = dir;

  if(dir>180)  dir = dir-360;
  if(dir<-180) dir = dir+360;

  // モーター出力を計算
  double p_gain = 1;
  double i_gain = 1;
  double d_gain = 1;

  static double i = 0;
  static double d = 0;

  i += dir;
  d = dir - prev_dir;

  double power = p*p_gain + i*i_gain + d*d_gain;

  prev_dir = dir;

  // motor_raw[0] = power;
  // motor_raw[1] = power;
  // motor_raw[2] = power;
  // motor_raw[3] = power;

  Serial.printf("dir:%d power:%d\n", dir, power);
  
  motorRaw();

}