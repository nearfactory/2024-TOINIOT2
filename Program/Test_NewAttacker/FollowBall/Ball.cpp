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

  float x2=0, y2=0;
  double dir2 = 0.0;

  for(int i=0;i<NUM;i++){
    ball[i] = analogRead(PIN[i]);
    distance += ball[i];

    float sensor_dir = radians(i*360/NUM);
    x += (1023 - ball[i]) * cos(sensor_dir);
    y += (1023 - ball[i]) * sin(sensor_dir);

    x2 += (1023 - ball[i])*(1023 - ball[i]) * cos(sensor_dir);
    y2 += (1023 - ball[i])*(1023 - ball[i]) * sin(sensor_dir);
  }

  // Serial.print("min:0 max:1023 ");
  for(int i=0;i<NUM;i++){
    // Serial.printf("b%d:%04d ", (i-3+16)%NUM, ball[(i-3+16)%NUM]);
    // Serial.printf("%04d ", ball[i]);
    // Serial.printf("%04d ", ball[i] - ball[(i+1)%16]);
  }

  int32_t v = 0;
  for(int i=0;i<16;i++){
    int d = ball[i]-ball[(i+1)%16];
    v += d*d;
  }
  dir = -degrees(atan2(y, x));
  dir2 = -degrees(atan2(y2,x2));

  int plus_v = 0;
  int plus_num = 0;
  int minus_v = 0;
  int minus_num = 0;
  float plus_avr  = 0;
  float minus_avr = 0;

  for(int i=0;i<16;i++){
    float diff = ball[i] - ball[(i+1)%16];
    if(diff>0){
      plus_avr += diff;
      plus_num++;
    }else{
      minus_avr += diff;
      minus_num++;
    }
  }

  plus_avr  = plus_avr / plus_num;
  minus_avr = minus_avr / minus_num;

  float avr_add = plus_avr + minus_avr;

  float hosei = (v-90000)/2000.0;

  // Serial.printf("%f ", hosei);
  Serial.printf("%f ", dir);
  // Serial.printf("%f ", dir2);
  // Serial.printf("%f ", dir2 - hosei);
  Serial.printf("%f ", plus_avr);
  Serial.printf("%f ", minus_avr);
  Serial.printf("%f ", avr_add);
  Serial.printf("%f ", dir-avr_add);
  // Serial.printf("%f ", dir-)
  Serial.println();
  
  is_exist = distance < DISTANCE_MAX;
  // hold_time = millis() - hold_begin;

  // is_holding = distance < BALL_DISTANCE_MIN && abs(ball_dir)<30;
  // ボールの保持有無が切り替わる or 保持していない時
  // if(!(ball_holding&&prev_ball_holding)){
  //   hold_begin = millis();
  // }
  return;
}