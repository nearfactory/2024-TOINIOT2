#pragma once

constexpr uint8_t BALL_NUM = 16;
const uint8_t BALL_PIN[BALL_NUM] = {
  14, 15, 16, 17, 18, 19, 20, 21, 22, 23,  26, 27, 38, 39, 40, 41
  // A0, A1, A2, A3, A4, A5, A6, A7, A8, A9,  A12, A13, A14, A15, A16, A17
};


int ball[BALL_NUM] = { 1023 };


inline void ballSetup(){
  for(auto p:BALL_PIN) pinMode(p, INPUT);

  Serial.println("ball setup");
  return;
}

inline void ballUpdate(){
  for(int i=0;i<BALL_NUM;i++){
    ball[i] = analogRead(BALL_PIN[i]);
  }
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