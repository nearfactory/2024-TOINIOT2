// Teensy 4.1
// Master

#include<string>

#include<Adafruit_BNO055.h>
#include<Adafruit_Sensor.h>
#include<utility/imumaths.h>
#include<Wire.h>

constexpr uint8_t ESP32C3_ADDR = 0x32;

Adafruit_BNO055 bno055(55, 0x28, &Wire);

void setup() {
  Serial.begin(9600);
  Serial.println("Teensy4.1");

  Wire.begin();
  Wire.beginTransmission(ESP32C3_ADDR);
  Wire.write("Teensy4.1");
  Wire.endTransmission();

  if(!bno055.begin()) while(1);
  Serial.println("bno055 started!");
}

void loop() {
  // BNO
  static sensors_event_t orientationData;
  double x=0;
  bno055.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  x=orientationData.orientation.x;
  Serial.println(x);

  // Wire
  std::string str = std::to_string(x);
  Wire.beginTransmission(ESP32C3_ADDR);
  Wire.write(str.c_str());
  Wire.endTransmission();

  delay(100);
}
