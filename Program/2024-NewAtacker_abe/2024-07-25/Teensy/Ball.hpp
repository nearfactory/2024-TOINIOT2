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