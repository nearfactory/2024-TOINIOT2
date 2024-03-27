#include "pin.h"
#include "motor.h"
#include "sensor.h"
#include "tone.h"

#include "camera.h"

// Constants
#define BALL_FRONT_TOLERANCE 30

// Objects
PixyI2C pixy(0x54);

// Variables
short int mode;

void setup() {
  pinSetup();
  tone(bzPin, 440);
  digitalWrite(ledPin[2], 1);
  while(true){
    if(digitalRead(btnPin[0]) == 1){
      waitBtn(0);
      noTone(bzPin);
      mode = 0;
      break;
    }
    if(digitalRead(btnPin[1]) == 1){
      waitBtn(1);
      noTone(bzPin);
      mode = 1;
      break;
    }
  }
  digitalWrite(ledPin[2], 0);
}

void loop() {
  refreshBallPrint();
  
  if(mode == 0){
    switch(ballBig){
    case 0:
      digitalWrite(ledPin[0], 0);
      digitalWrite(ledPin[1], 0);
      tone(bzPin, soundPitch[7]);
      break;

    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
      digitalWrite(ledPin[0], 1);
      digitalWrite(ledPin[1], 0);
      tone(bzPin, soundPitch[4]);
      break;

    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
      digitalWrite(ledPin[0], 0);
      digitalWrite(ledPin[1], 1);
      tone(bzPin, soundPitch[0]);
      break;
    }
  }
  else if(mode == 1){
    switch(ballBig){
    case 0:
      if(BALL_FRONT_TOLERANCE * -1 < ball[1]-ball[15] && ball[1]-ball[15] < BALL_FRONT_TOLERANCE){
        digitalWrite(ledPin[0], 0);
        digitalWrite(ledPin[1], 0);
        digitalWrite(ledPin[2], 1);
        tone(bzPin, soundPitch[7]);
      }
      else if(ball[1]-ball[15] < BALL_FRONT_TOLERANCE * -1){
        digitalWrite(ledPin[0], 0);
        digitalWrite(ledPin[1], 1);
        digitalWrite(ledPin[2], 1);
        tone(bzPin, soundPitch[0]*2);
      }
      else{
        digitalWrite(ledPin[0], 1);
        digitalWrite(ledPin[1], 0);
        digitalWrite(ledPin[2], 1);
        tone(bzPin, soundPitch[4]*2);
      }
      break;

    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
      digitalWrite(ledPin[0], 1);
      digitalWrite(ledPin[1], 0);
      digitalWrite(ledPin[2], 0);
      tone(bzPin, soundPitch[4]);
      break;

    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
      digitalWrite(ledPin[0], 0);
      digitalWrite(ledPin[1], 1);
      digitalWrite(ledPin[2], 0);
      tone(bzPin, soundPitch[0]);
      break;
    }
  }
}