#include <Arduino.h>
#include <Adafruit_BNO055.h>
#include <Wire.h>

// #include "buzzer.hpp"
#include "ball.hpp"
#include "line.hpp"
#include "motor.hpp"
#include "UI.hpp"

Adafruit_BNO055 bno(55, 0x28, &Wire);

static double default_dir = 0.0;

void setup() {
  if(!bno.begin()) while(true);

  ballSetup();
  buttonSetup();
  buzzerSetup();
  LEDSetup();
  lineSetup();
  motorSetup();
  
  Serial.begin(115200);
  Serial.println("IOT2 2024-06-06 AtMega2560 Test");

  bool button[BTN_NUM]{false};
  while (true) {
    buttonUpdate(button);
    if (button[0]) {
      while (true){
        buttonUpdate(button);
        if (!button[0]){
          sensors_event_t orientationData{};
          bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
          default_dir = orientationData.orientation.x-180;
          
          break;
        }
      }
      break;
    }
  }

}

void loop() {
  static int count=0;
  int processing_start = millis();

  short  motor[MOTOR_NUM]{};

  bool  button[BTN_NUM]{false};
  float buzzer = 0.0;
  bool  led[LED_NUM]{false};
  buttonUpdate(button);

  // ボール追う
  short ball[BALL_NUM]{1023};
  ballUpdate(ball);

  int ball_dir=0;
  int ball_distance = 0;
  for(auto& b:ball) ball_distance += b;
  int ball_max = 1023;
  for(int i=0;i<BALL_NUM;i++){
    if(ball[i]<ball_max){
      ball_max=ball[i];
      ball_dir = i;
    }
  }
  // setMove(motor, ball_dir*360/16, 128, 100);
  switch(ball_dir){
    case 0:
      // motorRaw(motor, 1,1,-1,-1, 90);
      motorRaw(motor, DIR::FRONT, 90);
      break;
    case 1:
      // motorRaw(motor, 0,1,0,-1, 90);
      break;
    case 2:
    case 3:
      // motorRaw(motor, -1,1,1,-1, 90);
      break;
    case 4:
    case 5:
      // motorRaw(motor, -1,0,1,0, 90);
      break;
    case 6:
    case 7:
    case 9:
    case 10:
      // motorRaw(motor, -1,-1,1,1, 90);
      break;
    case 8:
      // motorRaw(motor, 1,-1,1,-1, 90);
      break;
    case 11:
    case 12:
      // motorRaw(motor, 0,-1,0,1, 90);
      break;
    case 13:
    case 14:
      // motorRaw(motor, 1,-1,-1,1, 90);
      break;
    case 15:
      // motorRaw(motor, 1,0,-1,0, 90);
      break;
  }


  // 姿勢制御
  sensors_event_t orientationData{};
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  double dir = orientationData.orientation.x - 180;

  setDir(motor, dir, default_dir, 100, 40);


  // ボールを見ていない
  if(ball_distance > 16310){
    for(auto& m:motor) m=0;
  }

  // 白線避け
  bool line[LINE_NUM]{false};
  lineUpdate(line);
  led[0] = false;
  // left
  for(int i=LINE::LEFT1;i<LINE::LEFT6;i++){
    int c = i-LINE::LEFT1+1;
    if(line[i]){
      //setMove(motor, 180.0, c*255/4, 75);
      motorRaw(motor, 1,-1,-1,1, 100);
      led[0] = true;
    }
  }
  // back
  for(int i=LINE::BACK1;i<LINE::BACK6;i++){
    int c = i-LINE::BACK1+1;
    if(line[i]){
      //setMove(motor, 180.0, c*255/4, 75);
      motorRaw(motor, 1,1,-1,-1, 100);
      led[0] = true;
    }
  }
  // right
  for(int i=LINE::RIGHT2;i<LINE::RIGHT6;i++){
    int c = i-LINE::RIGHT2+1;
    if(line[i]){
      //setMove(motor, 180.0, c*255/4, 75);
      motorRaw(motor, -1,1,1,-1, 100);
      led[0] = true;
    }
  }
  // front
  for(int i=LINE::FRONT1;i<LINE::FRONT4;i++){
    int c = i-LINE::FRONT1+1;
    if(line[i]){
      motorRaw(motor, -1,-1,1,1, 100);
      led[0] = true;
    }
  }
  led[0] = line[LINE::FRONT2];
  led[1] = line[LINE::FRONT3];
  led[2] = line[LINE::FRONT4];
  setMotor(motor);
  LEDWrite(led);
  buzzerWrite(buzzer);


  // lineDebug();

  // Serial.print("dir:");
  // Serial.println(dir);
  /*
  Serial.print("max:");
  Serial.print(1023*BALL_NUM);
  Serial.print("min:");
  Serial.println("0");
  Serial.print("distance:");
  Serial.println(ball_distance);
  Serial.print("ball_dir:");
  Serial.println(ball_dir*1023);
  */
  // Serial.print("\n");
  // front

  /*
  */
  /*
  Serial.print("processing time:");
  Serial.print((millis()-processing_start));
  Serial.println("(ms)");
  Serial.println("\n--------------------------------\n");
  */

  //setMotor(motor);
  count++;

  // delay(50);
}

// setMoveの中身を書く