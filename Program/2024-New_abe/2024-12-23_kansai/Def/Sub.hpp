#pragma once

#include <Arduino.h>

class Sub{
  int count = 0;

  static constexpr uint8_t STR_SIZE = 4;
  
  static constexpr uint8_t QUEUE_SIZE = 4;
  uint8_t queue[QUEUE_SIZE][2];
  uint8_t queue_id = 0;
public:
  uint8_t ball01k = 0;
  uint8_t ball02k = 0;

  bool is_hold = false;

  void begin();
  void read();
  void kick();
};

extern Sub sub;

/*
uint8_t ball01k = 0;
uint8_t ball02k = 0;
uint8_t volume  = 0;

uint32_t kicker_timer = 0;

// シリアル
while(Serial7.available() >= 4){
  ball01k = Serial7.read();
  ball02k = Serial7.read();
  volume  = Serial7.read();
  Serial7.read();
}

digitalWrite(LED_BUILTIN, LOW);
while(Serial7.available()){
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.print((char)Serial7.read());
}

// キッカー
Serial7.print('k');
if(camera.w > 100 && millis()-kicker_timer > 5000){
  kick();
  kicker_timer = millis();
}

if(millis() - kicker_timer < 1000){
  motor.moveDir(0,50);
}
*/