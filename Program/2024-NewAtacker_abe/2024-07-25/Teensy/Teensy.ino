// new atacker main program
// Teensy4.1
// 2024-07-30

#include <Wire.h>

#include "Ball.hpp"
#include "Communication.hpp"
#include "Dir.hpp"
#include "Kicker.hpp"
#include "Motor.hpp"
#include "UI.hpp"

using namespace std;

void setup() {
  communicationSetup();
  ballSetup();
  dirSetup();
  kickerSetup();
  motorSetup();
  UISetup();

  display.clearDisplay();
  printd(8,8,"TOINIOT2 Atacker 2024-07-30");
  Serial.println("TOINIOT2 Teensy4.1 2024-07-30");
  display.display();

  MODE = MODE::DIR;
  uint8_t system = 0, gyro = 0, accel = 0, mag = 0;
  while(system!=3 || gyro!=3 || mag!=3){
    display.clearDisplay();
    printd(8,8,"calibration...");

    // dirCalibration(&system, &gyro, &accel, &mag);
    bno.getCalibration(&system, &gyro, &accel, &mag);
    printd(32,24,"system:"+to_string(system));
    printd(32,32,"gyro  :"+to_string(gyro));
    printd(32,40,"accel :"+to_string(accel));
    printd(32,48,"mag   :"+to_string(mag));

    display.display();
  }
}

void loop() {
  auto begin_ms = millis();
  display.clearDisplay();

  buttonUpdate();

  ballUpdate();
  // auto dir = dirUpdate();
  // drawAngleLine(-ball_dir,24);

  Serial2Update();
  dirUpdate();

  // UI (display)
  if(buttonUp(4)) MODE = (MODE+1)%MODE_NUM;
  debugDisplay(MODE);

  // printd(8, 8, "process:"+std::to_string(millis()-begin_ms)+"(ms)");
  display.display();
  delay(50);
}