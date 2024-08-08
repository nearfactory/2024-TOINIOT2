#pragma once

constexpr uint8_t BALL_NUM = 16;
const uint8_t BALL_PIN[BALL_NUM] = {
  14, 15, 16, 17, 18, 19, 20, 21, 22, 23,  26, 27, 38, 39, 40, 41
  // A0, A1, A2, A3, A4, A5, A6, A7, A8, A9,  A12, A13, A14, A15, A16, A17
};
enum class BALL{
  DIR = 0,
  DIR2,
  DIR3
};

constexpr short BALL_DISTANCE_MIN = 7000;
constexpr short BALL_DISTANCE_MAX = 15800;
constexpr short BALL_DISTANCE_RANGE = BALL_DISTANCE_MAX-BALL_DISTANCE_MIN;


short ball[BALL_NUM] = { 1023 };
float ball_dir = 0;
float prev_ball_dir = 0;
short ball_distance = 0;
bool  ball_holding = false;
bool  ball_exist = true;

short   ball_big = 1023;
uint8_t ball_big_id = 0;

short   ball_small = 0;
uint8_t ball_small_id = 0;

bool prev_ball_holding = false;
uint32_t ball_hold_begin = 0;

inline void ballSetup(){
  for(auto p:BALL_PIN) pinMode(p, INPUT);

  Serial.println("ball setup");
  return;
}


inline float ballDirection(){
  short ball_x = 0;
  short ball_y = 0;

  for(int i=0;i<BALL_NUM;i++){
    double sensor_dir = (i-1)/8.0+3.14;
    ball_x += ball[i] * cos(sensor_dir*PI);
    ball_y += ball[i] * sin(sensor_dir*PI);
  }
  return atan2(ball_y, ball_x) * 180.0 / PI;
}

inline float ballDirection2(){
  short ball_big = 0;
  short ball_big_index = 0;
  for(int i=0;i<BALL_NUM;i++){
    if(ball_big < ball[i]){
      ball_big = ball[i];
      ball_big_index = i;
    }
  }
  
  short ball_x = 0;
  short ball_y = 0;

  for(int i=-2;i<=2;i++){
    int j = (ball_big_index + BALL_NUM + i) % BALL_NUM;
    double sensor_dir = (j-1)*360.0/16.0 + 90.0 - 67.5;

    ball_x += ball[j] * cos(sensor_dir/180.0*3.14);
    ball_y += ball[j] * sin(sensor_dir/180.0*3.14);
  }

  return atan2(ball_y, ball_x) * (180.0 / PI);
}

inline float ballDirection3(){
  short ball_big = 0;
  short ball_big_index = 0;
  for(int i=0;i<BALL_NUM;i++){
    if(ball_big < ball[i]){
      ball_big = ball[i];
      ball_big_index = i;
    }
  }

  short ball_x = 0;
  short ball_y = 0;

  for(int i=-3;i<=3;i++){
    int j = (ball_big_index + BALL_NUM + i) % BALL_NUM;
    double sensor_dir = (j-1)*360.0/16.0 + 90.0 - 67.5;
    ball_x += ball[j] * cos(sensor_dir/180.0*3.14);
    ball_y += ball[j] * sin(sensor_dir/180.0*3.14);
  }
  return atan2(ball_y, ball_x) * (180.0 / PI);
}

inline void ballUpdate(BALL mode){
  ball_small = 1023;
  ball_small_id = 0;
  ball_big = 0;
  ball_big_id = 8;
  ball_distance = 0;
  for(int i=0;i<BALL_NUM;i++){
    ball[i] = analogRead(BALL_PIN[i]);
    ball_distance += ball[i];
    if(ball_small>ball[i]){
      ball_small = ball[i];
      ball_small_id = i;
    }if(ball_big<ball[i]){
      ball_big = ball[i];
      ball_big_id = i;
    }
  }

  prev_ball_holding = ball_holding;
  ball_holding = ball_distance < BALL_DISTANCE_MIN && abs(ball_dir)<30;
  ball_exist = ball_distance < BALL_DISTANCE_MAX;
  // ball_holding 5200-6800
  // exist < 16300

  if(!(ball_holding&&prev_ball_holding)){
    ball_hold_begin = millis();
  }

  switch(mode){
    case BALL::DIR :
      ball_dir = ballDirection();
      break;
    case BALL::DIR2 :
      ball_dir = ballDirection2();
      break;
    case BALL::DIR3 :
      ball_dir = ballDirection3();
      break;
  }

  // prev_ball_dir = ball_dir;
  // short tolerance = 20;
  // if((abs(ball_dir)-abs(prev_ball_dir))>tolerance){
  //   if(ball_dir>prev_ball_dir) ball_dir = prev_ball_dir+tolerance;
  //   if(ball_dir<prev_ball_dir) ball_dir = prev_ball_dir-tolerance;
  // }

  return;
}