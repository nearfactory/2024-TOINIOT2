#include "pin.h"
#include "motor.h"
#include "sensor.h"
#include "tone.h"

int power = 80;
int time = 1000;

void setup() {
  pinSetup();
  motorSetup(10, 100, 100, 100, 100);

  tone(bzPin, 440);
  waitBtn(0);
  noTone(bzPin);
}

void loop() {
  // motorP(100, 100, 100, 100, power);
  motorP(100, 100, -100, -100, power);
  delay(time);
  // motorP(100, -100, -100, 100, power);
  // delay(time);
  // motorP(-100, -100, 100, 100, power);
  // delay(time);
  // motorP(-100, 100, 100, -100, power);
  // delay(time);
  // motorP(0, 100, 0, -100, power);
  // delay(time);
  // motorP(0, -100, 0, 100, power);
  // delay(time);
  // motorP(-100, 0, 100, 0, power);
  // delay(time);
  // motorP(100, 0, -100, 0, power);
  // delay(time);
}