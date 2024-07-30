#pragma once

constexpr uint8_t BUZZER_PIN = 33;

void UISetup(){
  pinMode(BUZZER_PIN, OUTPUT);

  return;
}