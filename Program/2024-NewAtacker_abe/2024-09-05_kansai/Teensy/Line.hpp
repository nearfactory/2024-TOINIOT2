#pragma once

constexpr uint8_t LINE_NUM = 32;
bool line[LINE_NUM]{false};
char line_str[5] = "";

inline void lineUpdate(){
  char c = 0;
  while(c!='b'){
    Serial1.read();
  }
  return;
}