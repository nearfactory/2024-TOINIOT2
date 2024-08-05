#pragma once

#include <Wire.h>

void communicationSetup(){
  // UART
  Serial.begin(115200);   // pc
  Serial1.begin(115200);  // line
  Serial2.begin(115200);  // sub
  // Serial8.begin(115200);  // camera
  

  // I2C
  Wire2.begin();


  // Line
  // Serial1.print("T");

  // Sub
  Serial2.print("T");


  Serial.println("communication setup");
  return;
}