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

    display.printd(32,24,"system: "+to_string(system));
    display.printd(32,32,"gyro  : "+to_string(gyro));
    display.printd(32,40,"accel : "+to_string(accel));
    display.printd(32,48,"mag   : "+to_string(mag));
    display.draw();
  }

  display.printd(8,56,"start");
  display.draw();
  while(!ui.buttonUp(0)){ ui.read(); }
  
  dir.setDefault();
  digitalWrite(LED_BUILTIN, LOW);
}

int h = 10;
float p_gain = 1.5f;
float d_gain = 3.5f;
float* gain_select = &p_gain;

void loop() {
  ball.read();
  dir.read();
  line.read();

  if(digitalRead(36)){
    ui.read();
    if(ui.buttonUp(0)) display.next();

    display.addValiables("p:"+to_string(p_gain), &p_gain);
    display.addValiables("d:"+to_string(d_gain), &d_gain);

    display.debug();
    display.draw();

    motor.set(0,0,0,0);
    // delay(100);
  }else{
    ui.read();
    if(ui.buttonUp(0)) display.next();

    display.addValiables("p:"+to_string(p_gain), &p_gain);
    display.addValiables("d:"+to_string(d_gain), &d_gain);

    display.debug();
    display.draw();

    // motor.set(0,0,0,0);
    // delay(100);
    // 回り込み (方法4)
    // https://yuta.techblog.jp/archives/40889399.html
    float r = 14200;  // 回り込み時の半径
    float theta = 0.0;
    float move_dir = 0.0f;

    if(abs(ball.dir)<h){
      move_dir = ball.dir * p_gain - d_gain*(ball.dir - ball.dir_prev);
      h = 45;
    }else if(ball.distance < r){
      theta = 90 + (r-ball.distance) * 90 / r;
      move_dir = ball.dir + (ball.dir>0?theta:-theta);
      h = 45;
    }else{
      theta = degrees(atan2(r, ball.distance));
      move_dir = ball.dir + (ball.dir>0?theta:-theta);
      h = 45;
    }
    motor.moveDir(move_dir, 90);

    // 姿勢制御を加算
    float pow = dir.dir*60.0/180.0;
    motor.add(pow, pow, pow, pow);

    // 白線避け
    if(line.on) motor.moveDir(line.dir+180, 100);

    // motor.set(100.0f,100.0f,-100.0f,100.0f);
  }


  motor.avr();
  motor.write();

  delay(15);
}
