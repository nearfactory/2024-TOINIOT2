#include "wiring.h"
#include "Ball.hpp"

extern float r;

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
  
  // 保持
  if(is_hold){
    // 持ち終わり
    if(abs(dir) > 30.0 || distance > r + 200){
      is_hold = false;
      not_hold_begin = millis();
    }else{
      // 持ち続けている
      hold_time = millis() - hold_begin;
      not_hold_time = 0;
    }
  }else{
    // 持ち始め
    if(distance < r - 450 && abs(dir) < 2.5){
      is_hold = true;
      hold_begin = millis();
    }else{
      // 持っていないのが継続している
      hold_time = 0;
      not_hold_time = millis() - not_hold_begin;
    }
  }

  is_prev_hold = is_hold;

  if(is_hold){
    if(!sub.is_hold || abs(dir) > 35.0){
      is_hold = false;
    }
  }else{
    if(sub.is_hold && abs(dir) < 15.0){
      is_hold = true;
    }
  }

  return;
}