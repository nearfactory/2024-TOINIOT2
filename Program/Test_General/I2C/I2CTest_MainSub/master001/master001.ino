// #include <Wire.h>

#define I2C_SDA 4
#define I2C_SCL 5

byte val = 2;

void setup() {
  Serial.begin(9600);
  Serial.println("start");
  // Wire.begin();
  // Wire.setPins(I2C_SDA, I2C_SCL);

}


void loop() {
  Serial.println("Wire.getClock()");
//   Wire.beginTransmission(0x00);
//   Wire.write(val);
//   Wire.endTransmission();
//   Serial.println(Wire.getClock());
}