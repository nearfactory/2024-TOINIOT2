#include "Kicker.hpp"

void Kicker::begin(){
  pinMode(PIN, OUTPUT);
  return;
}

void Kicker::kick(){
  if(millis()-timer > interval){
    timer = millis();
  }
  return;
}

void Kicker::write(){
  is_ready = millis() - timer > interval;
  if(millis()-timer < 40){
    digitalWrite(PIN, HIGH);
  }else{
    digitalWrite(PIN, LOW);
  }
  return;
}