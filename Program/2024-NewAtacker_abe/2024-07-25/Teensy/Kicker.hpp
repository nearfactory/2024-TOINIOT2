#pragma once

namespace{
  constexpr uint8_t KICKER_PIN = 32;
}


void kickerSetup(){
  pinMode(KICKER_PIN, OUTPUT);

  Serial.println("kicker setup");
  return;
}

void kick(){
  digitalWrite(KICKER_PIN, 1);
  // delay();
  digitalWrite(KICKER_PIN, 0);

  return;
}