// Teensy 4.1
// Master

#include<string>

#include<Adafruit_BNO055.h>
#include<Wire.h>

Adafruit_BNO055 bno(55,0x28,&Wire2);
constexpr uint8_t ESP32C3_ADDR = 0x32;

void setup() {
  Serial.begin(9600);
  Serial.println("Teensy4.1");

  // Wire.setSDA(24);
  // Wire.setSCL(25);
  Wire2.begin();
  // Wire2.beginTransmission(ESP32C3_ADDR);
  // Wire2.write("Teensy4.1");
  // Wire2.endTransmission();

  while(!bno.begin());
  Serial.println("end setup()");
}

void loop() {
  
  sensors_event_t o{};
  bno.getEvent(&o,Adafruit_BNO055::VECTOR_EULER);
  Serial.printf("%lf %lf %lf\n", o.orientation.x, o.orientation.y, o.orientation.z);

  delay(100);
}
