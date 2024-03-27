#include "pin.h"
#include "motor.h"

void setup() {
  pinSetup();
  motorSetup(100, 100, 100, 100, 100);

  tone(bzPin, 440);
  waitBtn(0);
  noTone(bzPin);
}

void loop() {
  motorP(0, 100, 0, 0);
}