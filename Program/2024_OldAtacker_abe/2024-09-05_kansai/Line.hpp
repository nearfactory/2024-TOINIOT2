#pragma once


constexpr int LINE_NUM = 22;
const uint8_t LINE_PIN[LINE_NUM] = { 26, 27, 28, 29,    42, 43, 44, 45, 46, 47,    36, 37, 38, 39, 40, 41,    30, 31, 32, 33, 34, 35 };

bool line_on = false;
bool line[LINE_NUM]{false};

enum LINE : uint8_t{
  FRONT1 = 0,
  FRONT2,
  FRONT3,
  FRONT4,
  LEFT1,
  LEFT2,
  LEFT3,
  LEFT4,
  LEFT5,
  LEFT6,
  BACK1,
  BACK2,
  BACK3,
  BACK4,
  BACK5,
  BACK6,
  RIGHT1, // broken
  RIGHT2,
  RIGHT3,
  RIGHT4,
  RIGHT5,
  RIGHT6,
};

inline void lineSetup(){
  for(auto p:LINE_PIN) pinMode(p, INPUT);
  return;
}

inline void lineUpdate(){
  for(int i=0;i<LINE_NUM;i++) line[i] = digitalRead(LINE_PIN[i]);
  return;
}