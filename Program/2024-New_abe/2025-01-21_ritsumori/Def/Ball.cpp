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
    // Serial.printf("%d\t", ball[i]);
  }
  // Serial.println();

  for(int i=0;i<NUM;i++){
    float sensor_dir = radians(i*360/NUM);
    x += (1023 - ball[i]) * cos(sensor_dir);
    y += (1023 - ball[i]) * sin(sensor_dir);
  }

  dir_prev = dir;
  dir = -degrees(atan2(y, x));

  // 平均
  float diff = dir - dir_prev;
  queue[id] = abs(diff);
  id = (id+1) % 20;

  diff = 0;
  for(auto q:queue) diff += q;
  diff_avr = diff / 20.0f;

  
  is_exist = distance < DISTANCE_MAX;
  


  is_hold_prev = is_hold;
  is_hold = sub.is_hold;
  // 保持時間
  if(is_hold_prev == false && is_hold == true){
    hold_begin = millis();
  }
  if(is_hold){
    hold_time = millis()-hold_begin;
  }else{
    hold_time = 0;
  }


  return;
}