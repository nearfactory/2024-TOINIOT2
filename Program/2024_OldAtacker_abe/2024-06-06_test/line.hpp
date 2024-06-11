#pragma once
#ifndef LINE_NOUSE
constexpr int LINE_NUM = 22;
const uint8_t LINE_PIN[LINE_NUM] = { 26, 27, 28, 29,    42, 43, 44, 45, 46, 47,    36, 37, 38, 39, 40, 41,    30, 31, 32, 33, 34, 35 };
enum class LINE_PIN : uint8_t{
  FRONT1 = 26,
  FRONT2,
  FRONT3,
  FRONT4,

  LEFT1 = 42,
  LEFT2,
  LEFT3,
  LEFT4,
  LEFT5,
  LEFT6,

  BACK1 = 36,
  BACK2,
  BACK3,
  BACK4,
  BACK5,
  BACK6,

  RIGHT1 = 30,  // broken
  RIGHT2,
  RIGHT3,
  RIGHT4,
  RIGHT5,
  RIGHT6,
};

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


void lineSetup(){
  for(auto n:LINE_PIN) pinMode(n,INPUT);

  return;
}

void lineDebug(){
  Serial.print("line debug:");
  for(auto x:LINE_PIN) Serial.print(digitalRead(x));
  Serial.println();

  return;
}


void lineUpdate(bool *line){
  for(int i=0;i<LINE_NUM;i++){
    line[i] = digitalRead(LINE_PIN[i]);
  }

  return;
}

#endif