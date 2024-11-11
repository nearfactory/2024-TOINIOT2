#pragma once

using namespace std;


class UI
{
private:
  // pin
  constexpr uint8_t BUTTON_NUM = 4;
  const     uint8_t BUTTON_PIN[BUTTON_NUM] = { 37,31,30,32 };
  constexpr uint8_t TOGGLE_PIN = 36;
  constexpr uint8_t BZ_PIN = 33;

  // date
  bool button[BUTTON_NUM] = {false};
  bool button_prev[BUTTON_NUM] = {false};

  float bz = -1.0f;
public:
  UI();
  ~UI();

  void setup();
  void buttonRead();
  bool buttonUp(uint8_t id);
  void buzzer(float tone);
};

void UI::setup(){
  for(auto p:BUTTON_PIN) pinMode(p, INPUT);
  pinMode(TOGGLE_PIN, INPUT_PULLUP);
  pinMode(BZ_PIN, OUTPUT);
  
  Serial.println("ui ok");
  return;
}

void UI::buttonRead(){
  for(int i=0;i<BUTTON_NUM;i++){
    button_prev[i] = button[i];
    button[i] = digitalRead(BUTTON_PIN[i]);
  }
  return;
}

bool UI::buttonUp(uint8_t num){
  num = num<0 ? 0 : num;
  num = num>3 ? 3 : num;
  return (!button[num]) && button_prev[num];
}

void UI::buzzer(float tone){
  tone(BZ_PIN, bz);
  return;
}