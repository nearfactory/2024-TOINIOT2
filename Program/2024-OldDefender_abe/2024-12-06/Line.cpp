#include "Line.hpp"

void Line::begin(){
  for(auto p:PIN) pinMode(p, INPUT);

  return;
}

void Line::read(){
  for(int i=0;i<NUM;i++){
    line[i] = digitalRead(PIN[i]);
  }
  
  front = line[0]  | line[1]  | line[2]  | line[3];
  left  = line[4]  | line[5]  | line[6]  | line[7]  | line[8]  | line[9];
  back  = line[10] | line[11] | line[12] | line[13] | line[14] | line[15];
  right = line[16] | line[17] | line[18] | line[19] | line[20] | line[21];

  return;
}