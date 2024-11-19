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
  ball.begin();
  // dir.begin();
  // line.begin(115200);
  // motor.begin();
  
  // uint8_t system=0, gyro=0, accel=0, mag=0;
  // system = 3; gyro = 3; mag = 3;
  // while(system<3 || gyro<3 || mag<3){
  //   dir.calibration(&system, &gyro, &accel, &mag);
  //   digitalWrite(LED_BUILTIN, HIGH);
  // }

  // delay(1000);
  // dir.setDefault();
  // digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  ball.read();
  // Serial.println(ball.dir);
  // Serial.print("min:-180 max:180 dir:");
  // Serial.printf("dir:%f\n", ball.dir);

  /*
  if(digitalRead(TOGGLE_PIN)){
    motor.set(0,0,0,0);
    delay(50);
  }else{
    // 回り込み (方法4)
    // https://yuta.techblog.jp/archives/40889399.html
    float r = 8000;  // 回り込み時の半径
    float theta = 0.0;
    float move_dir = 0.0f;

    if(abs(ball.dir)<30){
      move_dir = ball.dir * 1.5;
    }else if(ball.distance < r){
      theta = 90 + (r-ball.distance) * 90 / r;
      move_dir = ball.dir + (ball.dir>0?theta:-theta);
    }else{
      theta = asin(r/ball.distance);
      move_dir = ball.dir + (ball.dir>0?theta:-theta);
    }
    motor.moveDir(move_dir, 80);

    // 姿勢制御を加算
    dir.read();
    // motor.setDir(dir.dir, 20.0);
    float pow = dir.dir*60.0/180.0;
    motor.add(pow, pow, pow, pow);

    // 白線避け
    line.read();
    if(line.on) motor.moveDir(line.dir+180, 80);

    Serial.printf("dir:%f line:%f\n", dir.dir, line.distance);
  }

  motor.avr();
  motor.write();

  delay(20);

  */
  delay(50);
}
