#include "Kicker.hpp"

void Kicker::begin(){
  pinMode(KICKER_PIN, OUTPUT);
  return;
}

void Kicker::kick(){
  if(millis()-kicked_timer > 5000){
    kicked_timer = millis();
  }
  return;
}

void Kicker::write(){
  if(millis()-kicked_timer < 50){
    digitalWrite(KICKER_PIN, HIGH);
  }else{
    digitalWrite(KICKER_PIN, LOW);
  }
  return;
}