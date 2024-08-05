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