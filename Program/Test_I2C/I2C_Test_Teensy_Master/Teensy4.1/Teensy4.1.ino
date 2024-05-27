// Teensy 4.1
// Master

#include<string>

#include<Adafruit_BNO055.h>
#include<Adafruit_GFX.h>
#include<Adafruit_Sensor.h>
#include<Adafruit_SSD1306.h>
#include<cmath>
#include<utility/imumaths.h>
#include<Wire.h>

constexpr uint8_t ESP32C3_ADDR = 0x32;

constexpr int DISPLAY_W = 128;
constexpr int DISPLAY_H = 64;
constexpr uint8_t DISPLAY_ADDR = 0x3c;
constexpr int OLED_RESET = -1;

Adafruit_BNO055 bno055(55, 0x28, &Wire);
Adafruit_SSD1306 display(DISPLAY_W, DISPLAY_H, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);
  Serial.println("Teensy4.1");

  Wire.begin();
  Wire.beginTransmission(ESP32C3_ADDR);
  Wire.write("Teensy4.1");
  Wire.endTransmission();
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDR)){
    Serial.println("failed to begin display!");
    while(1);
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(20,10);
  display.print("Teensy4.1");
  display.display();

  if(!bno055.begin()) while(1);
  Serial.println("bno055 started!");
}

void loop() {
  int previous_ms = millis();

  // BNO
  static sensors_event_t orientationData;
  bno055.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  double x=orientationData.orientation.x;


  // display
  
  display.clearDisplay();
  //display.setRotation(static_cast<uint8_t>(x*256/360));
  display.setCursor(20, 10);
  display.print(x);

  double x1 = std::cos((360-x)/180*PI) * 20 + DISPLAY_W/2;
  double y1 = std::sin((360-x)/180*PI) * 20 + DISPLAY_H/2;
  double x2 = DISPLAY_W / 2;
  double y2 = DISPLAY_H / 2;
  display.drawLine(x1, y1, x2, y2, SSD1306_WHITE);
  display.display();


  // Wire
  std::string str = std::to_string(x);
  Wire.beginTransmission(ESP32C3_ADDR);
  Wire.write(str.c_str());
  Wire.endTransmission();


  Serial.printf("angle:%lf, ms:%d\n", x, millis()-previous_ms);

  delay(100);
}
