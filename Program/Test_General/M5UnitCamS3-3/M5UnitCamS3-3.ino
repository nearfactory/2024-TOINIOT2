#include <SoftwareSerial.h>

#define SOFT_RXD 19
#define SOFT_TXD 20
SoftwareSerial mySerial(SOFT_RXD, SOFT_TXD);

void setup() {
  // mySerial.begin(9600, SERIAL_8N1, SOFT_RXD, SOFT_TXD, false, 256);
  mySerial.begin(9600);
}

void loop() {
  mySerial.println("M5 Stamp Cam S3");
  delay(50);
}