//ESP32C3
//Slave, Peripheral

#include <string>

#include <Wire.h>

std::string CIPO_string="";

constexpr uint8_t I2C_ADDR = 0x32;

void setup() {
  //Serial
  Serial.begin(9600);
  delay(1000);
  Serial.println("ESP32C3");
  
  //I2C
  Wire.begin(I2C_ADDR);
  Wire.onReceive(recieveEvent);
}

void loop() {
  Serial.println("a");
  Serial.println(CIPO_string.c_str());
}

void recieveEvent(int number) {
  Serial.println(number);
  CIPO_stri ng = "";
  while (Wire.available() > 0) {
    char c = Wire.read();
    CIPO_string += c;
  }

  return;
}