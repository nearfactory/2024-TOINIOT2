#pragma once

// Line - Serial1

constexpr uint8_t LINE_NUM = 32;
constexpr uint8_t LINE_CENTRAL = 20;
constexpr uint8_t LINE_OUTSIDE = 12;
bool line_on = false;
bool line[LINE_NUM]{true};  // debug
char line_str[5] = "";
float line_dir = 0.0f;
namespace{
  constexpr char BEGIN_CODE = 0b1010101010;
  constexpr char END_CODE   = 0b0101010101;
}

inline void lineSetup(){
  

  return;
}

// 終了コードなくても動く？
inline void lineUpdate(){
  char c = 0;
  // 開始コード
  while(c != BEGIN_CODE){
    if(Serial1.available()) c = Serial1.read();
  }
  // 4文字読み出す
  for(int i=0;i<4;i++){
    if(Serial1.available()) line_str[i] = Serial1.read();
  }
  while(c != END_CODE){
    if(Serial1.available()) c = Serial1.read();
  }

  // ---- ---- ---- ----
  for(int i=0;i<LINE_NUM;i++){
    line[i] = line_str[i/8] & 0b00000001<<(i%8);
  }

  // 角度計算

  return;
}