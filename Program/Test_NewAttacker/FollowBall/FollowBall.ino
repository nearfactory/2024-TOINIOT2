// new atacker main program
// Teensy4.1
// 2024-09-11

#include <Wire.h>

#include "Ball.hpp"
#include "Dir.hpp"
#include "Display.hpp"
#include "Line.hpp"
#include "Motor.hpp"
#include "UI.hpp"

Ball ball;
Dir dir;
Display display;
Line line;
Motor motor;
UI ui;

using namespace std;

void setup() {
  Serial.begin(115200);
  Serial.println("begin");

  ball.begin();
  display.begin();
  dir.begin();
  line.begin(115200);
  motor.begin();
  
  uint8_t system=0, gyro=0, accel=0, mag=0;
  // system = 3; gyro = 3; mag = 3;
  while(system<3 || gyro<3 || mag<3){
    dir.calibration(&system, &gyro, &accel, &mag);
    digitalWrite(LED_BUILTIN, HIGH);
  }

  delay(1000);
  dir.setDefault();
  digitalWrite(LED_BUILTIN, LOW);
}

int h = 45;

void loop() {
  ball.read();

  if(digitalRead(36)){
    display.debug(0);
    display.draw();
    // display.drawAngleLine(64, 32, ball.dir+180.0, 30);
    // display.draw();

    motor.set(0,0,0,0);
    delay(50);
  }else{
    // 回り込み (方法4)
    // https://yuta.techblog.jp/archives/40889399.html
    float r = 9000;  // 回り込み時の半径
    float theta = 0.0;
    float move_dir = 0.0f;

    if(abs(ball.dir)<h){
      move_dir = ball.dir * 1.5;
      h = 60;
    }else if(ball.distance < r){
      theta = 90 + (r-ball.distance) * 90 / r;
      move_dir = ball.dir + (ball.dir>0?theta:-theta);
      h = 45;
    }else{
      theta = asin(r/ball.distance);
      move_dir = ball.dir + (ball.dir>0?theta:-theta);
      h = 45;
    }
    motor.moveDir(move_dir, 90);

    // 姿勢制御を加算
    dir.read();
    // motor.setDir(dir.dir, 20.0);
    float pow = dir.dir*60.0/180.0;
    motor.add(pow, pow, pow, pow);

    // 白線避け
    line.read();
    if(line.on) motor.moveDir(line.dir+180, 100);

    // Serial.printf("dir:%f line:%f\n", dir.dir, line.distance);
  }

  motor.avr();
  motor.write();

  delay(15);
}
