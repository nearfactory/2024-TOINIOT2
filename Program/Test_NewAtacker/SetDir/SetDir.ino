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

  // モーター出力を計算
  double dir_power = dir / 1.8;
  Serial.println(dir_power);

  for(int i=0;i<MOTOR_NUM;i++) motor[i] = dir_power;
  motorP();
  motorRaw();

}