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

    float sensor_dir = radians(i*360/NUM);
    x += (1023 - ball[i]) * cos(sensor_dir);
    y += (1023 - ball[i]) * sin(sensor_dir);

    x2 += (ball[(i+1)%16]-ball[i]) * cos(sensor_dir);
    y2 += (ball[(i+1)%16]-ball[i]) * sin(sensor_dir);
  }

  int32_t v = 0;
  for(int i=0;i<16;i++){
    int d = ball[i]-ball[(i+1)%16];
    v += d*d;
  }
  dir = -degrees(atan2(y, x));
  dir2 = -degrees(atan2(y2, x2));

  for(int i=0;i<16;i++){
    Serial.printf("%d\t", ball[(i-8+16)%16]);
  }
  Serial.printf("%d\t", max_id);
  Serial.println();
  
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