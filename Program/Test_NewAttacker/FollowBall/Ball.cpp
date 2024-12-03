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

  // int left_val = 0;
  // int right_val = 0;
  // int center_val = 0;

  for(int i=0;i<7;i++){
    int j=(max_id-3+i+16)%16;

    // if(i==0) left_val = ball[j];
    // if(i==3) center_val = ball[j];
    // if(i==6) right_val = ball[j];

    float sensor_dir = radians(j*360/NUM);
    x += (1023 - ball[j]) * cos(sensor_dir);
    y += (1023 - ball[j]) * sin(sensor_dir);

    // if(i==0||i==3||i==6) Serial.printf("%d\t", ball[j]);
  }

  // double diff = (double)left_val - right_val;
  // if(diff>0) diff = diff/(left_val-center_val);
  // else       diff = diff/(right_val-center_val);

  dir = -degrees(atan2(y, x));

  // Serial.printf("%f\t%f\t%d\t", dir, diff*100, max_id);
  // Serial.println();
  
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