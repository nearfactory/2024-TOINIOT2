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


short ball[BALL_NUM] = { 1023 };
float ball_dir = 0;
short ball_distance = 0;
bool  ball_holding = false;

short   ball_big = 1023;
uint8_t ball_big_id = 0;

short   ball_small = 0;
uint8_t ball_small_id = 0;


inline void ballSetup(){
  for(auto p:BALL_PIN) pinMode(p, INPUT);

  Serial.println("ball setup");
  return;
}


inline float ballDirection(){
  short ball_x = 0;
  short ball_y = 0;

  for(int i=0;i<BALL_NUM;i++){
    double sensor_dir = (i-1)*360.0/16.0 + 180.0 - 67.5;
    ball_x += ball[i] * cos(sensor_dir/180.0*PI);
    ball_y += ball[i] * sin(sensor_dir/180.0*PI);
  }
  return atan2(ball_y, ball_x) * 180.0 / PI - 90.0;
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

  // ball_holding

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

  return;
}