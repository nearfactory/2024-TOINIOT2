// new atacker main program
// Teensy4.1
// 2024-07-30

#include <Wire.h>

#include "Ball.hpp"
#include "Dir.hpp"
#include "line.hpp"
#include "Motor.hpp"
#include "UI.hpp"

using namespace std;

void setup() {
  Serial.begin(9600);
  ballSetup();
  dirSetup();
  lineSetup();
  motorSetup();
  UISetup();

  // calibration
  uint8_t system = 0, gyro = 0, accel = 0, mag = 0;
  while(system!=3 || gyro!=3 || mag!=3){
    bno.getCalibration(&system, &gyro, &accel, &mag);
    buzzerUpdate();
    Serial.print(system);
    Serial.print(gyro);
    Serial.print(mag);
    Serial.println();
  }
  buzzerUpdate(0.0f);

  while(buttonUp(1)){
    buttonUpdate();
  }


  // set default dir
  dirUpdate();
  default_dir = dir;
  Serial.println("setup()");
}

void loop() {
  auto begin_ms = millis();

  ballUpdate(BALL::DIR);
  buttonUpdate();

  dirUpdate();
  
  // Serial.print("dir:");
  // Serial.println(dir);
  Serial.print("ball_dir:");
  Serial.println(ball_dir);
  Serial.print("distance:");
  Serial.print(ball_distance);

  buzzerUpdate(440+ball_dir);
  Serial.println("\n");
  delay(50);
}