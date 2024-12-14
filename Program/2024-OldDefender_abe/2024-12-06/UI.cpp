#include "UI.hpp"

void UI::begin(){
  for(auto p:BUTTON_PIN) pinMode(p, INPUT);
  pinMode(TOGGLE_PIN, INPUT_PULLUP);
  pinMode(BZ_PIN, OUTPUT);

  Serial.println("ui setup");
  
  return;
}

void UI::read(){
  for(int i=0;i<BUTTON_NUM;i++){
    previous_button[i] = button[i];
    button[i] = digitalRead(BUTTON_PIN[i]);
  }
  return;
}

bool UI::buttonUp(uint8_t id){
  id = id<0 ? 0 : id;
  id = id>3 ? 3 : id;

  return (!button[id]) && previous_button[id];
}

void UI::buzzer(float frequency){
  bz = frequency;
  tone(BZ_PIN, bz);

  return;
}