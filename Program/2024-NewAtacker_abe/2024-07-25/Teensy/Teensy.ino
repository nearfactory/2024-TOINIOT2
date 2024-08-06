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
  ballSetup();
  communicationSetup();
  dirSetup();
  kickerSetup();
  motorSetup();
  UISetup();

  /*
  // calibration
  DISPLAY_MODE = DISPLAY_MODE::DIR;
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
  */
}

void loop() {
  auto begin_ms = millis();
  display.clearDisplay();

  ballUpdate(BALL::DIR);

  subUpdate();
  dirUpdate();

  analogWrite(0,0);

  motorRaw(100,-100,100,100);


  buttonUpdate();
  // UI (display)
  clearVariables();
  addVariables("process", begin_ms-millis());
  if(buttonUp(4)) DISPLAY_MODE = (DISPLAY_MODE+1)%DISPLAY_MODE_NUM;
  debugDisplay(DISPLAY_MODE);
  display.display();

  // printd(8, 8, "process:"+std::to_string(millis()-begin_ms)+"(ms)");
}