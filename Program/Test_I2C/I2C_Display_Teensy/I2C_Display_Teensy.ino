// Teensyはリセットボタンを押しながらUSBを挿してから書き込むと治る

#include<Adafruit_BNO055.h>
#include<Adafruit_GFX.h>
#include<Adafruit_Sensor.h>
#include<Adafruit_SSD1306.h>
#include<cmath>
#include<Wire.h>

constexpr int DISPLAY_W = 128;
constexpr int DISPLAY_H = 64;
constexpr uint8_t DISPLAY_ADDR = 0x3c;
constexpr int OLED_RESET = -1;

Adafruit_BNO055 bno(55, 0x28, &Wire);
Adafruit_SSD1306 display(DISPLAY_W, DISPLAY_H, &Wire, OLED_RESET);

void setup(){
  Serial.begin(9600);
  Wire.begin();

  if(!display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDR)){
    Serial.println("failed to begin display!");
    while(1);
  }
  if(!bno.begin()){
    Serial.println("failed to begin BNO055");
    while(1);
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.display();

  Serial.println("display begin!");
  delay(1000);
}

void loop(){
  int previous_ms = millis();
  sensors_event_t orientation;
  bno.getEvent(&orientation, Adafruit_BNO055::VECTOR_EULER);
  double x = orientation.orientation.x;

  display.clearDisplay();
  //display.setRotation(static_cast<uint8_t>(x*256/360));
  display.setCursor(20, 10);
  display.print(x);
  double angle1 = (0.0 + x);
  double angle2 = (120.0+x);
  double angle3 = (240.0+x);

  int x1 = std::cos(angle1) * 20 + DISPLAY_W/2;
  int x2 = std::cos(angle2) * 20 + DISPLAY_W/2;
  int x3 = std::cos(angle3) * 20 + DISPLAY_W/2;

  int y1 = std::sin(angle1) * 20 + DISPLAY_H/2;
  int y2 = std::sin(angle2) * 20 + DISPLAY_H/2;
  int y3 = std::sin(angle3) * 20 + DISPLAY_H/2;

  //display.drawTriangle(0, 0, 0, DISPLAY_H, DISPLAY_W, DISPLAY_H, SSD1306_WHITE);
  //display.drawTriangle(x1, y1, x2, y2, x3, y3, SSD1306_WHITE);

  x1 = std::cos((360-x)/180*PI) * 20 + DISPLAY_W/2;
  y1 = std::sin((360-x)/180*PI) * 20 + DISPLAY_H/2;
  x2 = DISPLAY_W / 2;
  y2 = DISPLAY_H / 2;
  display.drawLine(x1, y1, x2, y2, SSD1306_WHITE);
  display.display();

  Serial.printf("angle:%lf, seconds:%d", x, millis()-previous_ms);
  delay(100);
}