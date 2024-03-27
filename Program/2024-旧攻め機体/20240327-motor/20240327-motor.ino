#include "pin.h"
#include "motor.h"
#include "tone.h"

void setup() {
  pinSetup();
  motorSetup(10, 100, 100, 100, 100);

  // tone(bzPin, soundPitch[9]);
  // waitBtn(0);
  // noTone(bzPin);
}

void loop() {
  mpCalc(100, 100, -100, -100);
  move();

  for(int i=0; i<4; i++){
    Serial.print(mpOutput[i]);
    Serial.print("\t");
  }
  Serial.print(100);
  Serial.print("\t");
  Serial.println(-100);
}