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

}

void loop() {
  auto begin_ms = millis();
  display.clearDisplay();


  ballUpdate();
  auto ball_dir = ballDirection();
  auto dir = dirUpdate();


  printd(8, 8, "process:"+std::to_string(millis()-begin_ms)+"(ms)");
  display.display();
  // delay(50);
}