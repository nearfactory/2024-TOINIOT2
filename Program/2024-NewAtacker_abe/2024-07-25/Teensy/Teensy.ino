// new atacker main program
// Teensy4.1
// 2024-07-30

#include <Wire.h>

#include "Ball.hpp"
#include "Communication.hpp"
#include "Kicker.hpp"
#include "Motor.hpp"
#include "UI.hpp"

void setup() {
  communicationSetup();
  ballSetup();
  kickerSetup();
  motorSetup();
  UISetup();

  Serial.println("TOINIOT2 Teensy4.1 2024-07-30");

}

void loop() {
  ballUpdate();
}