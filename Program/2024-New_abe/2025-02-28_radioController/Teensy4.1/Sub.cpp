#include "Sub.hpp"

void Sub::begin(){
  Serial7.begin(115200);
  return;
}


void Sub::read(){
  if(Serial7.available()){
    str = "";
    while(Serial7.available()){
      char c = (char)Serial7.read();
      str += c;
      Serial.print(c);
    }
  }

  return;
}


void Sub::kick(){
  bool ready = millis()-kicked_timer > INTERVAL;

  if(ready){
    Serial7.print('k');
    kicked_timer = millis();
  }

  return;
}