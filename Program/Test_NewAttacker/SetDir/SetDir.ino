#include <Arduino.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>

#include "Motor.hpp"

Adafruit_BNO055 bno(55, 0x28, &Wire2);
double default_dir = 0;

Adafruit_SSD1306 display(128, 64, &Wire2, -1);

void setup(){
  Serial.begin(9600);
  Serial.println("TOINIOT2 SetDir Test 2024-09-04");

  Wire2.begin();

  if(!bno.begin()){
    Serial.println("bno err!");
    while(true);
  }

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)){
    Serial.println("display err!");
    while(true);
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.display();

  Serial.println("setup()");

  delay(500);
  sensors_event_t orientation;
  bno.getEvent(&orientation, Adafruit_BNO055::VECTOR_EULER);
  default_dir = orientation.orientation.x;
}

void loop(){
  sensors_event_t orientation;
  bno.getEvent(&orientation, Adafruit_BNO055::VECTOR_EULER);
  double dir = orientation.orientation.x - default_dir;
  static double prev_dir = dir;

  if(dir>180)  dir = dir-360;
  if(dir<-180) dir = dir+360;

  // モーター出力を計算
  static double p_gain = 1.4;
  static double i_gain = 0.3;
  static double d_gain = 0.8;

  static double p = 0;
  static double i = 0;
  static double d = 0;

  p = 0 - dir;
  i += dir;
  d = dir - prev_dir;

  double power = p*p_gain + i*i_gain + d*d_gain;

  prev_dir = dir;

  // motor_raw[0] = power;
  // motor_raw[1] = power;
  // motor_raw[2] = power;
  // motor_raw[3] = power;

  Serial.printf("dir:%d power:%d\n", dir, power);
  
  // motorRaw();

  // display
  display.clearDisplay();
  display.println("val:");
  display.printf("p:%lf\ni:%lf\nd:%lf\n", p, i, d);
  display.println("gain:");
  display.printf("p:%lf\ni:%lf\nd:%lf\n", p_gain, i_gain, d_gain);
  display.display();
}