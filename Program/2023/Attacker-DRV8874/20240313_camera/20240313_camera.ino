#include "pin.h"
#include "sensor.h"
#include "camera.h"

// Object of Camera(Pixy1)
PixyI2C pixy(0x54);

void setup() {
  pinSetup();
  cameraSetup(pixy, 100);
}

void loop() {
  refreshCamera(pixy);
  Serial.println(blockNum);
  if(blockNum > 0){
    digitalWrite(ledPin[2], 1);
  }
  else{
    digitalWrite(ledPin[2], 0);
  }
}
