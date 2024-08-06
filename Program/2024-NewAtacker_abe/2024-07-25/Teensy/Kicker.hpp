#pragma once

namespace{
  constexpr uint8_t KICKER_PIN = 32;
}


inline void kickerSetup(){
  pinMode(KICKER_PIN, OUTPUT);

  Serial.println("kicker setup");
  return;
}

inline void kick(){
  digitalWrite(KICKER_PIN, 1);
  delay(50);
  digitalWrite(KICKER_PIN, 0);

  return;
}