#include "Camera.hpp"

Camera camera;

void setup(){
  Serial.begin(115200);
  Serial.println("TOINIOT2 Pixy2 Test 2024-12-15");
  
  camera.begin();
}

void loop(){
  camera.read();
}