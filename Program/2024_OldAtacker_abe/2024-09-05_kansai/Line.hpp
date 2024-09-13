#pragma once


constexpr int LINE_NUM = 22;
const uint8_t LINE_PIN[LINE_NUM] = { 26, 27, 28, 29,    42, 43, 44, 45, 46, 47,    36, 37, 38, 39, 40, 41,    30, 31, 32, 33, 34, 35 };

bool line_on = false;
bool line[LINE_NUM]{false};  // debug
char line_str[5] = "";
float line_dir = 0.0f;
namespace{
  constexpr char BEGIN_CODE = 0b1010101010;
  constexpr char END_CODE   = 0b0101010101;
}

inline void lineSetup(){
  for(auto p:LINE_PIN) pinMode(p, INPUT);
  return;
}

inline void lineUpdate(){
  for(int i=0;i<LINE_NUM;i++) line[i] = digitaLRead(LINE_PIN[i]);
  return;
}