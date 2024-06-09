#include <Arduino.h>
#include <Adafruit_BNO055.h>
#include <Wire.h>

#include "ball.hpp"
#include "buzzer.hpp"
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
  
  Serial.begin(9600);
  Serial.println("IOT2 2024-06-06 AtMega2560 Test");

  bool button[BTN_NUM]{false};
  while (true) {
    buttonUpdate(button);
    if (button[0]) {
      while (true){
        buttonUpdate(button);
        if (!button[0]) break;
      }
      break;
    }
  }

  sensors_event_t orientationData{};
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  default_dir = orientationData.orientation.x;
  
}

void loop() {
  static int count=0;
  /*
  static int processing_start = 0;
  processing_start = millis();
  */

  static uint8_t  motor[MOTOR_NUM]{0};
  
  static int    ball_dir=0;
  static int    ball_distance = 0;
  static double dir = 0;

  // ballUpdate(ball);
  // buttonUpdate(button);
  // static bool     button[BTN_NUM]{false};
  // static float    buzzer = 0.0;
  // static bool     LED[LED_NUM]{false};
  // ballDebug();
  // motorDebug();
  
  // ボール追う
  static short ball[BALL_NUM]{1023};
  ballUpdate(ball);

  for(auto& b:ball) ball_distance += b;
  int ball_max = 0;
  for(int i=0;i<BALL_NUM;i++){
    if(ball[i]<ball_max){
      ball_max=ball[i];
      ball_dir = i;
    }
  }
  setMove(motor, ball_dir*360/16, 128, 100);

  // 姿勢制御
  static sensors_event_t orientationData{};
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  dir = orientationData.orientation.x;

  setDir(motor, dir, default_dir, 255, 50);


  // ボールを見ていない
  if(ball_distance > 16000){
    for(auto& m:motor) m=0;
  }


  // 白線避け
  static bool line[LINE_NUM]{false};
  lineUpdate(line);

  Serial.print("max:");
  Serial.print(1023*BALL_NUM);
  Serial.print("min:");
  Serial.println(0);
  Serial.print("distance:");
  Serial.print(ball_distance);
  Serial.print("\nball_dir:");
  Serial.print(ball_dir);
  Serial.print("\n");
  // front
  for(int i=LINE::FRONT1;i<LINE::FRONT4;i++){
    int c = i-LINE::FRONT1+1;
    if(line[i]){
      setMove(motor, 180.0, c*255/4, 75);
    }
  }
  // left
  for(int i=LINE::LEFT1;i<LINE::LEFT6;i++){
    int c = i-LINE::LEFT1+1;
    if(line[i]){
      setMove(motor, 180.0, c*255/4, 75);
    }
  }
  // back
  for(int i=LINE::BACK1;i<LINE::BACK6;i++){
    int c = i-LINE::BACK1+1;
    if(line[i]){
      setMove(motor, 180.0, c*255/4, 75);
    }
  }
  // right
  for(int i=LINE::RIGHT1;i<LINE::RIGHT6;i++){
    int c = i-LINE::RIGHT1+1;
    if(line[i]){
      setMove(motor, 180.0, c*255/4, 75);
    }
  }

  /*
  Serial.print("processing time:");
  Serial.print((millis()-processing_start));
  Serial.println("(ms)");
  Serial.println("\n--------------------------------\n");
  */

  setMotor(motor);
  count++;

  delay(50);
}

// ボタンを押して離して動き出すとok
// ボールを隠して止まるとok
// BNOの値の向きを確認する

// setMove, setDir の中身を書く