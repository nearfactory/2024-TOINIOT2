// debug
#define MOTOR_NOUSE

#include<string.h>
#include <Arduino.h>
#include "ball.hpp"
#include "buzzer.hpp"
#include "line.hpp"
#include "motor.hpp"

void setup() {
  Serial.begin(9600);
  Serial.println("IOT2 2024-06-06 AtMega2560 Test");

  ballSetup();
  lineSetup();
  buzzerSetup();
  // motorSetup();
}

void loop() {
  static int count=0;
  static int processing_start = 0;
  processing_start = millis();

  static short ball[BALL_NUM]{1023};
  static bool line[LINE_NUM]{false};
  
  // lineUpdate(line);
  // ballUpdate(ball);
  // ballDebug();
  // motorDebug();

  ballUpdate(ball);
  int distance = 0;
  for(auto& x:ball) distance+=x;
  int ball_max = 0;
  int dir=0;
  for(int i=0;i<BALL_NUM;i++){
    if(ball[i]>ball_max){
      ball_max=ball[i];
      dir = i;
    }
  }
  Serial.print("max:");
  Serial.print(1023*BALL_NUM);
  Serial.print("min:");
  Serial.println(0);
  Serial.print("distance:");
  Serial.print(distance);
  Serial.print("\ndir:");
  Serial.print(dir);
  Serial.print("\n");

  /*
  Serial.print("processing time:");
  Serial.print((millis()-processing_start));
  Serial.println("(ms)");
  Serial.println("\n--------------------------------\n");
  */

  count++;

  delay(50);
}