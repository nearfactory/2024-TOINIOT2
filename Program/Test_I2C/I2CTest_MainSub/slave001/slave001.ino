#include <Wire.h>

#define I2C_SDA 18
#define I2C_SCL 19

void setup() {
  Serial.begin(9600);
  Serial.println("start");

  Wire.begin();

  Wire.setSDA(I2C_SDA);
  Wire.setSDA(I2C_SCL);
}

void loop() {
  Wire.requestFrom(0x00, 1);
  Serial.println("doing");

  while (Wire.available()) {
    byte val = Wire.read();
    Serial.print(val);
  }

  delay(250);
}