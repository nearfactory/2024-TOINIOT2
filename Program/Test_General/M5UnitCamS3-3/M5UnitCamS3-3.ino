// #include <SoftwareSerial.h>

// #define SOFT_RXD 19
// #define SOFT_TXD 20
// SoftwareSerial Serial;

void setup() {
  // Serial.begin(115200, SWSERIAL_8N1, SOFT_RXD, SOFT_TXD, false, 256);
  Serial.begin(115200);
}

void loop() {
  Serial.println("M5 Stamp Cam S3");
  delay(50);
}