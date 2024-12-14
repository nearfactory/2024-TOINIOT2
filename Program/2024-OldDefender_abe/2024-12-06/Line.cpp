#include "Line.hpp"

void Line::begin(){
  for(auto p:PIN) pinMode(p, INPUT);

  return;
}

void Line::read(){
  for(int i=0;i<NUM;i++){
    line[i] = digitalRead(PIN[i]);
  }
}