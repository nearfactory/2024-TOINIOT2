#include "pin.h"
#include "motor.h"
#include "tone.h"

int mode = 0;

void setup() {
  pinSetup();
  motorSetup(10, 100, 100, 100, 100);

  tone(bzPin, soundPitch[9]);
  waitBtn(0);
  noTone(bzPin);
}

void loop() {
  if(digitalRead(btnPin[0]) == 1){
    waitBtn(0);
    mode = 0;
  }
  if(digitalRead(btnPin[1]) == 1){
    waitBtn(1);
    mode = 1;
  }
  if(digitalRead(btnPin[2]) == 1){
    waitBtn(2);
    mode = 2;
  }

  switch(mode){
  case 0:
    mpCalc(100, 100, -100, -100);
    break;
  
  case 1:
    mpCalc(-100, -100, 100, 100);
    break;

  case 2:
    mpCalc(100, -100, -100, 100);
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