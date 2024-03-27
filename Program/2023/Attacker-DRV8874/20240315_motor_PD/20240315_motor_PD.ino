#include "pin.h"
#include "motor.h"
#include "sensor.h"
#include "tone.h"

void setup() {
  pinSetup();
  motorSetup(10, 100, 90, 100, 90);

  tone(bzPin, 440);
  waitBtn(0);
  noTone(bzPin);
}

void loop() {
  motorP(-100, -100, 100, 100, 80);
  delay(2000);
  motorP(100, 100, -100, -100, 80);
  delay(2000);
}