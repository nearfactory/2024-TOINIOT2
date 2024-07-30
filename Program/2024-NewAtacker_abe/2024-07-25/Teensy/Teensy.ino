// Teensy4.1
// 2024-07-30

#include <Wire.h>

#include "Ball.hpp"
#include "Motor.hpp"
#include "UI.hpp"

void setup() {
  ballSetup();
  motorSetup();
  UISetup();


  // Serial
  Serial.begin(9600);
  Serial.println("TOINIOT2 Teensy4.1 2024-07-30");
  // Line
  // Serial1.begin(9600);
  // Serial1.print("T");
  // Sub
  Serial2.begin(9600);
  Serial2.print("T");

  // I2C
  Wire2.begin();

}

void loop() {
  

}