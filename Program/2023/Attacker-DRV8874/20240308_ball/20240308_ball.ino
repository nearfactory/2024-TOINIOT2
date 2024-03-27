#include "pin.h"
#include "motor.h"
#include "sensor.h"
#include "tone.h"

void setup() {
  pinSetup();
  motorSetup(10, 100, 100, 100, 100);

  tone(bzPin, 440);
  waitBtn(0);
  noTone(bzPin);
}

void loop() {
  refreshLinePrint();

  if(lineBlock[4] != 0){
    motorP(0, 0, 0, 0);
    delay(250);
    return;
  }

  refreshBall();

  // motorP(100, 100, 100, 100);

  switch(ballBig){
  case 0:
  case 1:
  case 15:
    motorP(100, 100, -100, -100);
    break;

  case 2:
  case 3:
  case 4:
    motorP(-100, 0, 100, 0);
    break;

  case 5:
  case 6:
  case 10:
  case 11:
    motorP(-100, -100, 100, 100);
    break;

  case 7:
  case 8:
  case 9:
    motorP(100, -100, -100, 100);
    break;

  case 12:
  case 13:
  case 14:
    motorP(0, -100, 0, 100);
    break;
  }
}
