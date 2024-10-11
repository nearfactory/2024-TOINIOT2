#pragma once

#include <string>

#include <Wire.h>


std::string BLE_atk{};
std::string BLE_def{};

inline void communicationSetup(){
  // UART
  Serial.begin(115200);   // pc
  Serial1.begin(115200);  // line
  Serial2.begin(115200);  // sub
  Serial8.begin(115200);  // camera
  

  // I2C
  Wire2.begin();


  // Line
  // Serial1.print("T");

  // Sub
  // Serial2.print("T");


  Serial.println("communication setup");
  return;
}


inline void subUpdate(){
  if(Serial2.available()){
    BLE_def = "";
    while(Serial2.available()){
      BLE_def += static_cast<char>(Serial2.read());
    }
  }
  Serial2.println(BLE_atk.c_str());

  return;
}


inline void cameraUpdate();