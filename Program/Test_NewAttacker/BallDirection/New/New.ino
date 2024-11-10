#include<cmath>

#include"UI.hpp"

constexpr int BALL_NUM = 16;
const uint8_t BALL_PIN[BALL_NUM] = {
  14, 15, 16, 17, 18, 19, 20, 21, 22, 23,  26, 27, 38, 39, 40, 41
};

void setup() {
  Serial.begin(115200);
  Serial.println("Ball Direction Test Program 2024-07-27");

  UISetup();

  for(auto p:BALL_PIN) pinMode(p,INPUT);
}

// ボールを一番見ているセンサーの左右ひとつづつの数値の偏りを比較する

void loop() {
  unsigned short ball[BALL_NUM]{0};

  double ball_x = 0, ball_y = 0;
  for(int i=0;i<BALL_NUM;i++){
    ball[i] = analogRead(BALL_PIN[i]);

    double sensor_dir = radians(i*360.0/16.0);
    ball_x+=ball[i]*cos(sensor_dir);
    ball_y+=ball[i]*sin(sensor_dir);

    Serial.printf("%04d ", ball[i]);
  }
  double ball_dir = degrees(atan2(ball_y,ball_x));
  Serial.println();


  display.clearDisplay();
  drawAngleLine(64,30,ball_dir,30);
  display.display();

  delay(25);
}
