#pragma once

namespace{
  constexpr uint8_t KICKER_PIN = 32;
}

uint32_t kicked_ms = 0;
short ball01k = -1;
short ball02k = -1;

inline void kickerSetup(){
  pinMode(KICKER_PIN, OUTPUT);

  Serial.println("kicker setup");
  return;
}

inline void kick(){
  digitalWrite(KICKER_PIN, HIGH);
  delay(50);
  digitalWrite(KICKER_PIN, LOW);
  kicked_ms = millis();
  return;
}