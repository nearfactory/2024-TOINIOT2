#include "pin.h"
#include "motor.h"
#include "sensor.h"
#include "tone.h"

#define DIR_TOLERANCE 7
#define BALL_MIN 100

#define DIR_GAIN 0.85

short state;

void setup() {
  pinSetup();
  motorSetup(10, 100, 100, 100, 100);

  tone(bzPin, soundPitch[9]);
  Serial.println("2024-08-07 OldAttacker-Defender");
  delay(50);
  noTone(bzPin);

  dirSetup();

  while(true){
    refreshDir();
    Serial.println(dir);
    if(dir < 9000){
      digitalWrite(ledPin[0], 0);
      
      Serial.println("Calibration Completed!");
      dirCalibration = true;
    }
    else{
      digitalWrite(ledPin[0], 1);
    }

    if(digitalRead(btnPin[0])){
      while(digitalRead(btnPin[0]));
      dirOpponent = dir;
      break;
    }
  }

  while(!digitalRead(btnPin[1]));
  while(digitalRead(btnPin[1]));
}

void loop() {
  if(digitalRead(btnPin[3])){
    mpCalc(0, 0, 0, 0);
    move();
    return;
  }
  // 入力
  refreshDir();
  if(abs(dir) < DIR_TOLERANCE){
    refreshLine();
    if(lineBlock[4] == 0){
      refreshBall();
      state = 2;
    }
    else{
      state = 1;
    }
  }
  else{
    state = 0;
  }

  // 計算
  switch(state){
  case 0:
    // Dir
    dir = dir>100 ? 100 : dir;
    dir = dir<-100 ? -100 : dir;
    mpCalc(-1*dir*DIR_GAIN, -1*dir*DIR_GAIN, -1*dir*DIR_GAIN, -1*dir*DIR_GAIN);
    break;

  case 1:
    // Line
    switch(lineBlock[4]){
    case 1000:
      // front
      mpCalc(-100, -100, 100, 100);
      break;
    
    case 0100:
      // Left
      mpCalc(100, -100, -100, 100);
      break;

    case 0010:
      // Back
      mpCalc(100, 100, -100, -100);
      break;
    
    case 0001:
      // Right
      mpCalc(-100, 100, 100, -100);
      break;

    case 1100:
      mpCalc(0, -100, 0, 100);
      break;

    case 0110:
      mpCalc(100, 0, -100, 0);
      break;

    case 0011:
      mpCalc(0, 100, 0, -100);
      break;

    case 1001:
      mpCalc(-100, 0, 100, 0);
      break;

    default:
      mpCalc(0, 0, 0, 0);
      break;
    }
    break;
  

  case 2:
    // Ball
    if(ballSum < BALL_MIN){
      // Can't find ball
      mpCalc(0, 0, 0, 0);
      break;
    }
    switch(ballBig){
    case 0:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
      mpCalc(0, 0, 0, 0);
      break;

    case 1:
      mpCalc(-50, 50, 50, -50);
      break;

    case 2:
      mpCalc(-75, 75, 75, -75);

    case 3:
    case 4:
      mpCalc(-100, 100, 100, -100);
      break;

    case 12:
    case 13:
      mpCalc(100, -100, -100, 100);
      break;

    case 14:
      mpCalc(75, -75, -75, 75);
      break;

    case 15:
      mpCalc(50, -50, -50, 50);
      break;    
    }
    break;

  default:
    mpCalc(0, 0, 0, 0);
    break;
  }

  // 出力
  move();

  // for(int i=0; i<4; i++){
  //   Serial.print(mpOutput[i]);
  //   Serial.print("\t");
  // }
  // Serial.print(state);
  // Serial.print("\n");
}
