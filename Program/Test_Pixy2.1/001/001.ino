#include "camera.h"

// Constants
PixyI2C pixy(0x54);

void setup() {
  cameraSetup(pixy, 100);
  Serial.begin(115200);
}

void loop() {
  Serial.println(122);
  refreshCamera(pixy);

  Serial.print(blockX);
  Serial.print("\t");
  Serial.println(blockWidth);

  delay(100);
}