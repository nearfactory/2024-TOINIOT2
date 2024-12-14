#include "wiring.h"
#include "Ball.hpp"

void Ball::begin(){
  for(auto p:PIN) pinMode(p, INPUT);
  Serial.println("ball setup");

  return;
}

void Ball::read(){
  float x=0, y=0;
  distance = 0;

  int max = 1023;
  int max_id = 0;

  float x2=0, y2=0;
  double dir2 = 0.0;

  for(int i=0;i<NUM;i++){
    ball[i] = analogRead(PIN[i]);
    distance += ball[i];

    if(max>ball[i]){
      max = ball[i];
      max_id = i;
    }
  }

  for(int i=0;i<16;i++){
    float sensor_dir = radians(i*360/NUM);
    x += (1023 - ball[i]) * cos(sensor_dir);
    y += (1023 - ball[i]) * sin(sensor_dir);
  }

  dir = -degrees(atan2(y, x));
  dir_prev = dir;

  
  is_exist = distance < DISTANCE_MAX;
  /*
  hold_time = millis() - hold_begin;

  is_holding = distance < BALL_DISTANCE_MIN && abs(ball_dir)<30;
  ボールの保持有無が切り替わる or 保持していない時
  if(!(ball_holding&&prev_ball_holding)){
    hold_begin = millis();
  }
  */
  return;
}