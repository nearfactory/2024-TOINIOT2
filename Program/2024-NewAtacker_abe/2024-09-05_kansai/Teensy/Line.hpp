#pragma once

constexpr uint8_t LINE_NUM = 32;
constexpr uint8_t LINE_CENTRAL = 20;
constexpr uint8_t LINE_OUTSIDE = 12;
bool line[LINE_NUM]{true};  // debug
char line_str[5] = "";

inline void lineUpdate(){
  char c = 0;
  while(c!='b'){
    Serial1.read();
  }
  return;
}