#include "pin.h"
#include "motor.h"
#include "tone.h"
#include "sensor.h"

// Constant
constexpr int BALL_EXIST = 150;

// Variable
int mode = 0;

void setup() {
  pinSetup();
  motorSetup(10, 100, 100, 100, 100);

  tone(bzPin, soundPitch[9]);
  waitBtn(0);
  noTone(bzPin);
}

void loop() {
  refreshBall();

  if(ballSum < BALL_EXIST){
    mpCalc(0, 0, 0, 0);
    move();
    return;
  }

  switch(ballBig){
  case 0:
    mpCalc(100, 100, -100, -100);
    break;

  case 1:
  case 2:
  case 3:
    mpCalc(-100, 0, 100, 0);
    break;

  case 4:
  case 5:
  case 6:
  case 10:
  case 11:
  case 12:
    mpCalc(-100, -100, 100, 100);
    break;

  case 7:
  case 8:
  case 9:
    mpCalc(-100, 100, 100, -100);
    break;

  case 13:
  case 14:
  case 15:
    mpCalc(0, -100, 0, 100);
    break;
  }

  move();

  for(int i=0; i<4; i++){
    Serial.print(mpOutput[i]);
    Serial.print("\t");
  }
  Serial.print(100);
  Serial.print("\t");
  Serial.println(-100);
}