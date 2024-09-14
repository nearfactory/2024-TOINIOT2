// 2024-07-07

#include <Arduino.h>
#include <Adafruit_BNO055.h>
#include <Wire.h>

#include "ball.hpp"
#include "line.hpp"
#include "motor.hpp"
#include "UI.hpp"

Adafruit_BNO055 bno(55, 0x28, &Wire);

static double default_dir = 0.0;

void setup() {
  if(!bno.begin()) while(true);

  ballSetup();
  buttonSetup();
  buzzerSetup();
  LEDSetup();
  lineSetup();
  motorSetup();
  
  Serial.begin(9600);
  Serial.println("IOT2 2024-07-07 AtMega2560 Test");

  bool button[BTN_NUM]{false};
  while (true) {
    buttonUpdate(button);
    if (button[0]) {
      while (true){
        buttonUpdate(button);
        if (!button[0]){
          sensors_event_t orientationData{};
          bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
          default_dir = orientationData.orientation.x-180;
          
          break;
        }
      }
      break;
    }
  }
  // 左 5 6  右 2  後 2 5
}

void loop() {
  lineDebug();
}

// 白線避け
// ボール追い