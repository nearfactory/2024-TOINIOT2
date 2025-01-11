#include "pin.h"
#include "motor.h"
#include "sensor.h"
#include "tone.h"
#include "camera.h"

Pixy2I2C pixy;

// Constants
// Dir
#define DIR_TOLERANCE 10
#define P_GAIN_DIR 0.95
#define D_GAIN_DIR 0.7

// Ball
#define BALL_EXIST 100

// Variables

// Dir
float dirPrevious = 0;
float diffDiff = 0;
float mpDir;

// motorPower
int mpD = 100;
int mpL = 80;
int mpB = 100;
int mpP = 40;

// Timer
unsigned long start;
//unsigned long ton0;
//unsigned long toff0;

//ボールキックカウンター
int i;
int j;
int k;


void setup() {
  pinSetup();
  dirSetup();
  motorSetup(10, 100, 100, 100, 100);
  cameraSetup(pixy, 100);

  digitalWrite(ledPin[0], 1);

  while (true) {
    refreshDirRaw();
    if (dir > 9000) {
      tone(bzPin, soundPitch[9]);
    } else {
      noTone(bzPin);
      dirCalibration = true;
    }

    if (digitalRead(btnPin[0]) == 1) {
      waitBtn(0);
      dirOpponent = dir;
      break;
    }
  }

  digitalWrite(ledPin[0], 0);
  start = millis();
  //ton0 = millis();  //始めの時刻を覚えておく
}

void loop() {

  // Camera
  refreshCamera(pixy);
  Serial.print(blockX);
  Serial.print("\t");
  Serial.println(blockWidth);

  //unsigned long t = millis();

  if (digitalRead(btnPin[3]) == 1) {
    motor(0, 0, 0, 0, 100);
    digitalWrite(ledPin[0], 1);
    digitalWrite(ledPin[1], 1);
    digitalWrite(ledPin[2], 1);
    return;
  }

  if (digitalRead(btnPin[1]) == 1) {
    while(lineBlock[0] == 0) {
      motor(50, 100, -50, -100, mpL);
      digitalWrite(ledPin[1], 1);
      refreshLine();
      //delay(400);
    }
    motor(-50, -100, 40, 90, mpL);
    delay(200);
    return;
  }

  if (digitalRead(btnPin[2]) == 1) {
    while(lineBlock[0] == 0) {
      motor(50, 100, -50, -100, mpL);
      digitalWrite(ledPin[1], 1);
      refreshLine();
      //delay(400);
    }
    motor(-50, -100, 40, 90, mpL);
    delay(200);
    return;
  }

  //-------------------------------------------

  /*if(blockNum == 0){
        if(analogRead(A1) + analogRead(A2) + analogRead(A3) + analogRead(A4) + analogRead(A5) + analogRead(A6) + analogRead(A7) > analogRead(A9) + analogRead(A10) + analogRead(A11) + analogRead(A12) + analogRead(A13) + analogRead(A14) + analogRead(A15)){
          tone(bzPin, soundPitch[4]);
          motor(50, -100, -50, 100, mpL); //左斜め後ろに進む
        }
        else{
          tone(bzPin, soundPitch[5]);
          motor(-100, -50, 100, 50, mpL); //右斜め後ろに進む
        }
      }
  else{
    tone(bzPin, soundPitch[6]);
  }
  */
  // ========================================

  // Line
  refreshLine();
  if(lineBlock[7] == 2) {
    digitalWrite(ledPin[1], 1);
    switch (lineBlock[7]) {
    //case 1111111:  //all
    //case 1000100:  //front_back in
    //case 1001000:  //front_back out
      //motor(0, 0, 0, 0, mpL);
      //tone(bzPin, soundPitch[9]);
      //break;


    case 1000000:  //front
    //case 1001100:  //front_back out in
      motor(100, 100, -85, -85, mpL);
      delay(20);
      return;
    
    case 10000:   //left in
    case 100000:  //left out
    case 101000:  //left out_back out
    case 101100:  //left out_back out in
    case 100100:  //left out_back in
      //motor(100, 50, -100, -50, mpB); //左斜め前に進む
      //delay(300);
      if(blockNum == 2){
        if(analogRead(A1) + analogRead(A2) + analogRead(A3) + analogRead(A4) + analogRead(A5) + analogRead(A6) + analogRead(A7) > analogRead(A9) + analogRead(A10) + analogRead(A11) + analogRead(A12) + analogRead(A13) + analogRead(A14) + analogRead(A15)){
          tone(bzPin, soundPitch[3]);
          tone(bzPin, soundPitch[6]);
          motor(50, 100, -50, -100, mpL); //右斜め前に進む
          delay(100);
          return;
        }
        else{
          tone(bzPin, soundPitch[7]);
          tone(bzPin, soundPitch[10]);
          motor(100, 50, -100, -50, mpL); //左斜め前に進む
          delay(100);
          return;
        }
      }
      

    //case 0:
    //  motor(-80, -80, 80, 80, mpL);
    //  break;

    case 10001:  //left in_right in
    case 100010: //left out_right out
    case 110011: //left out in_right out in
      if(blockNum == 2){
        if(analogRead(A1) + analogRead(A2) + analogRead(A3) + analogRead(A4) + analogRead(A5) + analogRead(A6) + analogRead(A7) > analogRead(A9) + analogRead(A10) + analogRead(A11) + analogRead(A12) + analogRead(A13) + analogRead(A14) + analogRead(A15)){
          tone(bzPin, soundPitch[3]);
          tone(bzPin, soundPitch[6]);
          motor(50, 100, -50, -100, mpL); //右斜め前に進む
          delay(100);
          return;
        }
        else{
          tone(bzPin, soundPitch[7]);
          tone(bzPin, soundPitch[10]);
          motor(100, 50, -100, -50, mpL); //左斜め前に進む
          delay(100);
          return;
        }
      }


    
    case 1:     //right in
    case 10:    //right out
    case 110:   //back in_right out
    case 1110:  //back out in_right out
    case 1010:  //back out_right out
      //motor(50, 100, -50, -100, mpB); //右斜め前に進む
      //delay(300);
      if(blockNum == 2){
        if(analogRead(A1) + analogRead(A2) + analogRead(A3) + analogRead(A4) + analogRead(A5) + analogRead(A6) + analogRead(A7) > analogRead(A9) + analogRead(A10) + analogRead(A11) + analogRead(A12) + analogRead(A13) + analogRead(A14) + analogRead(A15)){
          tone(bzPin, soundPitch[3]);
          tone(bzPin, soundPitch[6]);
          motor(50, 100, -50, -100, mpL); //右斜め前に進む
          delay(100);
          return;
        }
        else{
          tone(bzPin, soundPitch[7]);
          tone(bzPin, soundPitch[10]);
          motor(100, 50, -100, -50, mpL); //左斜め前に進む
          delay(100);
          return;
        }
      }
      

    //case 1100000:  //front_left out
    //  motor(95, 95, -95, -100, mpL);
    //  tone(bzPin, soundPitch[9]);
    //  break;


    //case 1000010:  //front_right out
    //   motor(95, 95, -95, -100, mpL);
    //   tone(bzPin, soundPitch[9]);
    //   break;



    case 1100:   //back_in out
    case 1000:   //back_out
    case 100:    //back_in
      if(blockNum == 2){
        if(analogRead(A1) + analogRead(A2) + analogRead(A3) + analogRead(A4) + analogRead(A5) + analogRead(A6) + analogRead(A7) > analogRead(A9) + analogRead(A10) + analogRead(A11) + analogRead(A12) + analogRead(A13) + analogRead(A14) + analogRead(A15)){
          tone(bzPin, soundPitch[3]);
          tone(bzPin, soundPitch[6]);
          motor(-100, 40, 100, -40, mpL); //右斜め後ろに進む
          delay(100);
          return;
        }
        else{
          tone(bzPin, soundPitch[7]);
          tone(bzPin, soundPitch[10]);
          motor(40, -100, -40, 100, mpL); //左斜め後ろに進む
          delay(100);
          return;
        }
      }
      /*else{
        motor(50, 100, -40, -90, mpL);
        delay(200);
        return;
      }*/

    default:
      motor(0, 0, 0, 0, mpL);
      break;
    }
  
  }


  digitalWrite(ledPin[1], 0);

  //------------------------------------------------------------------

  // Dir
  while(true){
    refreshDir();
    if(-1 * DIR_TOLERANCE < dir && dir < DIR_TOLERANCE){
      break;
    }
    digitalWrite(ledPin[0], 1);
    mpDir = (dir * P_GAIN_DIR + diffDiff * D_GAIN_DIR) * (-1);
    motor(mpDir, mpDir, mpDir, mpDir, mpD);
    diffDiff = abs(dir) - abs(dirPrevious);
    dirPrevious = dir;
    start = millis();
  }

  digitalWrite(ledPin[0], 0);

  // ========================================
 

  // Ball
  refreshBall16();
  // if(ballSum < BALL_EXIST) {
  //   motor(0, 0, 0, 0, mpB);
  //   return;
  // }

  switch (ballBig) {
  case 0:
    if(blockNum == 2){
      if(analogRead(A0) > 310){
        motor(0, 0, 0, 0, mpB);
      }
      else{
        for(i =1; i <= 1000; i++){
          motor(0, 0, 0, 0, mpB);
          if(analogRead(A0) > analogRead(A1) || analogRead(A0) > analogRead(A15)){
            return;
          }
        }
        motor(50, 100, -40, -90, mpB);
        delay(250);
        if(lineBlock[0] + lineBlock[1] + lineBlock[2] + lineBlock[5] + lineBlock[6] == 0){
          motor(-50, -90, 40, 70, mpB);
          delay(280);
        }
        else{
            motor(50, 100, -40, -90, mpB);
            delay(250);
        }
      }
    }
    break;
  
 //---------------------↑case0　　case1~7↓------------------------------
  case 1:
  case 2:
  case 3:
  case 4:
  case 5:
  case 6:
  case 7:
    if(blockNum != 2){
      if(blockNum != 0){
        motor(50, -50, -50, 50, mpB);
        delay(50);
      }
      if(lineBlock[3] + lineBlock[4] + lineBlock[5] + lineBlock[6] == 0){
        motor(-100, -0, 100, 0, mpP);//右斜め後ろに進む
      }
      else{
        motor(-50, -90, 40, 70, mpL);
        delay(75);
        while(analogRead(A1) + analogRead(A2) + analogRead(A3) + analogRead(A4) + analogRead(A5) + analogRead(A6) + analogRead(A7) < analogRead(A9) + analogRead(A10) + analogRead(A11) + analogRead(A12) + analogRead(A13) + analogRead(A14) + analogRead(A15)){
          motor(0, 0, 0, 0, mpB);
          if(blockNum == 2){
            return;
          }
          if(analogRead(A1) < 300 || analogRead(A2) < 300){
            for(j = 1; j <= 1000; j++){
              motorP(0, 0, 0, 0, mpB);
              if(analogRead(A1) + analogRead(A2) > analogRead(A14) + analogRead(A15)){
                if(lineBlock[3] + lineBlock[4] == 2){
                  motor(50, 100, -40, -90, mpB);
                  delay(175);
                  motor(-30, 100, 30, -100, mpL); //右斜め前に進む
                  delay(400);
                }
                else{
                  motor(-30, 100, 30, -100, mpL); //右斜め前に進む
                  delay(450);
                }
                if(lineBlock[7] == 0){
                  while(lineBlock[7] == 0){
                    if(analogRead(A1) + analogRead(A2) + analogRead(A3) + analogRead(A4) + analogRead(A5) + analogRead(A6) + analogRead(A7) > analogRead(A9) + analogRead(A10) + analogRead(A11) + analogRead(A12) + analogRead(A13) + analogRead(A14) + analogRead(A15)){
                      motor(-100, 40, 100, -40, mpB); //右斜め後ろに進む
                    }
                    else{
                      motor(40, -100, -40, 100, mpB); //左斜め後ろに進む
                    } 
                  }
                }
                return;
              }
              if(analogRead(A1) + analogRead(A2) > analogRead(A3) + analogRead(A4)){
                while(analogRead(A1) + analogRead(A2) + analogRead(A3) + analogRead(A4) + analogRead(A5) + analogRead(A6) + analogRead(A7) < analogRead(A9) + analogRead(A10) + analogRead(A11) + analogRead(A12) + analogRead(A13) + analogRead(A14) + analogRead(A15)){
                  motor(0, 0, 0, 0, mpB);
                }
                if(lineBlock[3] + lineBlock[4] == 2){
                  motor(50, 100, -40, -90, mpB);
                  delay(175);
                  motor(-30, 100, 30, -100, mpL); //右斜め前に進む
                  delay(400);
                }
                else{
                  motor(-30, 100, 30, -100, mpL); //右斜め前に進む
                  delay(450);
                }
                if(lineBlock[7] == 0){
                  while(lineBlock[7] == 0){
                    if(analogRead(A1) + analogRead(A2) + analogRead(A3) + analogRead(A4) + analogRead(A5) + analogRead(A6) + analogRead(A7) > analogRead(A9) + analogRead(A10) + analogRead(A11) + analogRead(A12) + analogRead(A13) + analogRead(A14) + analogRead(A15)){
                      motor(-100, 40, 100, -40, mpB); //右斜め後ろに進む
                    }
                    else{
                      motor(40, -100, -40, 100, mpB); //左斜め後ろに進む
                    } 
                  }
                }
                return;
              }
            }
            motor(100, 0, -100, 0, mpB); //左斜め前
            delay(500);
            motor(-100, 40, 100, -40, mpB); //右斜め後ろ(コート中央に戻る)
            delay(500);
            return;
          }
        }
        if(lineBlock[3] + lineBlock[4] == 2){
          motor(50, 100, -40, -90, mpB);
          delay(175);
          motor(-30, 100, 30, -100, mpL); //右斜め前に進む
          delay(400);
        }
        else{
          motor(-30, 100, 30, -100, mpL); //右斜め前に進む
          delay(450);
        }
        if(lineBlock[7] == 0){
          while(lineBlock[7] == 0){
            if(analogRead(A1) + analogRead(A2) + analogRead(A3) + analogRead(A4) + analogRead(A5) + analogRead(A6) + analogRead(A7) > analogRead(A9) + analogRead(A10) + analogRead(A11) + analogRead(A12) + analogRead(A13) + analogRead(A14) + analogRead(A15)){
              motor(-100, 40, 100, -40, mpB); //右斜め後ろに進む
            }
            else{
              motor(40, -100, -40, 100, mpB); //左斜め後ろに進む
            } 
          }
        }
        return;
      }
    }
    else{
      motor(50, -100, -50, 100, mpB);
      tone(bzPin, soundPitch[0]);
      if(lineBlock[0] == 1){
        if(analogRead(A1) + analogRead(A2) + analogRead(A3) + analogRead(A4) + analogRead(A5) + analogRead(A6) + analogRead(A7) > analogRead(A9) + analogRead(A10) + analogRead(A11) + analogRead(A12) + analogRead(A13) + analogRead(A14) + analogRead(A15)){
          motor(30, 100, -30, -100, mpL); //右斜め前に進む
          delay(100);
        }
        else{
          motor(100, 30, -100, -30, mpL); //左斜め前に進む
          delay(100);
        }

      }
    }
    break;

 //---------------------↑case1~7　　case8↓------------------------------
  case 8:
    motor(0, 0, 0, 0, mpB);
    tone(bzPin, soundPitch[11]);
    break;

 //---------------------↑case8　　case9~15↓------------------------------
  case 9:
  case 10:
  case 11:
  case 12:
  case 13:
  case 14:
  case 15:
    if(blockNum != 2){
      if(blockNum != 0){
        motor(-50, 50, 50, -50, mpB);
        delay(50);
      }
      if(lineBlock[3] + lineBlock[4] + lineBlock[5] + lineBlock[6] == 0){
        motor(0, -100, -0, 100, mpP); //左斜め後ろに進む
      }
      else{
        motor(-50, -90, 40, 70, mpL);
        delay(75);
        while(analogRead(A1) + analogRead(A2) + analogRead(A3) + analogRead(A4) + analogRead(A5) + analogRead(A6) + analogRead(A7) > analogRead(A9) + analogRead(A10) + analogRead(A11) + analogRead(A12) + analogRead(A13) + analogRead(A14) + analogRead(A15)){
          motor(0, 0, 0, 0, mpB);
          if(blockNum == 2){
            return;
          }
          if(analogRead(A14) < 310 || analogRead(A15) < 310){
            for(k = 1; k <= 1000; k++){
              motorP(0, 0, 0, 0, mpB);
              if(analogRead(A14) + analogRead(A15) > analogRead(A1) + analogRead(A2)){
                if(lineBlock[3] + lineBlock[4] == 2){
                  motor(50, 100, -40, -90, mpB);
                  delay(175);
                  motor(100, -30, -100, 30, mpB); //左斜め前に進む
                  delay(350);
                }
                else{
                  motor(100, -30, -100, 30, mpB); //左斜め前に進む
                  delay(400);
                }
                if(lineBlock[7] == 0){
                  while(lineBlock[7] == 0){
                    if(analogRead(A1) + analogRead(A2) + analogRead(A3) + analogRead(A4) + analogRead(A5) + analogRead(A6) + analogRead(A7) > analogRead(A9) + analogRead(A10) + analogRead(A11) + analogRead(A12) + analogRead(A13) + analogRead(A14) + analogRead(A15)){
                      motor(-100, 40, 100, -40, mpB); //右斜め後ろに進む
                    }
                    else{
                      motor(40, -100, -40, 100, mpB); //左斜め後ろに進む
                    } 
                  }
                }
                return;
              }
              if(analogRead(A14) + analogRead(A15) > analogRead(A12) + analogRead(A13)){
                while(analogRead(A1) + analogRead(A2) + analogRead(A3) + analogRead(A4) + analogRead(A5) + analogRead(A6) + analogRead(A7) > analogRead(A9) + analogRead(A10) + analogRead(A11) + analogRead(A12) + analogRead(A13) + analogRead(A14) + analogRead(A15)){
                  motor(0, 0, 0, 0, mpB);
                }
                if(lineBlock[3] + lineBlock[4] == 2){
                  motor(50, 100, -40, -90, mpB);
                  delay(175);
                  motor(100, -30, -100, 30, mpB); //左斜め前に進む
                  delay(350);
                }
                else{
                  motor(100, -30, -100, 30, mpB); //左斜め前に進む
                  delay(400);
                }
                if(lineBlock[7] == 0){
                  while(lineBlock[7] == 0){
                    if(analogRead(A1) + analogRead(A2) + analogRead(A3) + analogRead(A4) + analogRead(A5) + analogRead(A6) + analogRead(A7) > analogRead(A9) + analogRead(A10) + analogRead(A11) + analogRead(A12) + analogRead(A13) + analogRead(A14) + analogRead(A15)){
                      motor(-100, 40, 100, -40, mpB); //右斜め後ろに進む
                    }
                    else{
                      motor(40, -100, -40, 100, mpB); //左斜め後ろに進む
                    } 
                  }
                }
                return;
              }
            }
            motor(0, 100, 0, -100, mpB); //右斜め前
            delay(500);
            motor(40, -100, -40, 100, mpB); //左斜め後ろ(コート中央に戻る)
            delay(500);
            return;
          }
        }
        if(lineBlock[3] + lineBlock[4] == 2){
          motor(50, 100, -40, -90, mpB);
          delay(175);
          motor(100, -30, -100, 30, mpB); //左斜め前に進む
          delay(350);
        }
        else{
          motor(100, -30, -100, 30, mpB); //左斜め前に進む
          delay(400);
        }
        if(lineBlock[7] == 0){
          while(lineBlock[7] == 0){
            if(analogRead(A1) + analogRead(A2) + analogRead(A3) + analogRead(A4) + analogRead(A5) + analogRead(A6) + analogRead(A7) > analogRead(A9) + analogRead(A10) + analogRead(A11) + analogRead(A12) + analogRead(A13) + analogRead(A14) + analogRead(A15)){
              motor(-100, 40, 100, -40, mpL); //右斜め後ろに進む
            }
            else{
              motor(40, -100, -40, 100, mpL); //左斜め後ろに進む
            } 
          }
        }
        return;
      }
    }
    else{
      motor(-100, 50, 100, -50, mpB);
      tone(bzPin, soundPitch[4]);
      if(lineBlock[0] == 1){
        if(analogRead(A1) + analogRead(A2) + analogRead(A3) + analogRead(A4) + analogRead(A5) + analogRead(A6) + analogRead(A7) > analogRead(A9) + analogRead(A10) + analogRead(A11) + analogRead(A12) + analogRead(A13) + analogRead(A14) + analogRead(A15)){
          motor(30, 100, -30, -100, mpL); //右斜め前に進む
          delay(100);
        }
        else{
          motor(100, 30, -100, -30, mpL); //左斜め前に進む
          delay(100);
        }

      }
    }
    break;
    
  }




}