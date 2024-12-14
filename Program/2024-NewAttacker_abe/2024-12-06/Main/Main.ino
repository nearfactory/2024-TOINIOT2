// new atacker main program
// Teensy4.1
// 2024-09-11

#include <Wire.h>

#include "Ball.hpp"
#include "Camera.hpp"
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
  /*
  Serial.println("begin");

  ball.begin();
  display.begin();
  dir.begin();
  line.begin(115200);
  */
  motor.begin();
  
  /*
  uint8_t system=0, gyro=0, accel=0, mag=0;
  // system = 3; gyro = 3; mag = 3;
  while(system<3 || gyro<3 || mag<3){
    dir.calibration(&system, &gyro, &accel, &mag);
    digitalWrite(LED_BUILTIN, HIGH);

    display.printd(32,16,"system: "+to_string(system));
    display.printd(32,24,"gyro  : "+to_string(gyro));
    display.printd(32,32,"accel : "+to_string(accel));
    display.printd(32,40,"mag   : "+to_string(mag));
    display.draw();
  }

  // 攻め方向が設定されるまで待機
  digitalWrite(LED_BUILTIN, LOW);
  display.printd(8,56,"set dir");
  display.draw();
  while(!ui.buttonUp(0)){ ui.read(); }
  
  dir.setDefault();
  */
}

bool display_on = true;

float r = 14400.0f;  // 回り込み時の半径
int h = 10;
float p_gain = 1.5f;
float d_gain = 3.5f;
float* gain_select = &p_gain;

float p1 = 8.5f;

float dir_threshold = 2.0f;

void loop() {
  /*
  ball.read();
  dir.read();
  line.read();

  if(digitalRead(36)){
    ui.read();
    if(ui.buttonUp(0)) display.next();

    // display.addValiables("p gain:"+to_string(p_gain), &p_gain);
    // display.addValiables("d gain:"+to_string(d_gain), &d_gain);
    display.addValiables("power :"+to_string(p1), &p1);
    display.addValiables("dir   :"+to_string(dir_threshold), &dir_threshold);
    display.addValiables("dir y :"+to_string(dir.dir_y), &dir.dir_y);
    display.addValiables("dir z :"+to_string(dir.dir_z), &dir.dir_z);
    // display.addValiables("radius:"+to_string(r), &r);

    display.debug();
    display.draw();

    motor.set(0,0,0,0);
    display_on = true;
  }else{
    // ディスプレイを消灯
    if(display_on){
      display.draw();
      display_on = false;
    }

    // 回り込み (方法4)
    // https://yuta.techblog.jp/archives/40889399.html
    float theta = 0;
    float move_dir = 0;

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
    motor.moveDir(move_dir, p1*10.0f);

    // ボールが見えない場合に後ろに下がる (デバッグ段階では手前に)
    if(!ball.is_exist){
      motor.moveDir(0, 60);
    }

    // 姿勢制御を加算
    float pow = dir.dir*60.0/180.0;
    motor.add(pow, pow, pow, pow);

    // 白線避け
    if(line.on) motor.moveDir(line.dir+180, 100);



    // コート橋の傾斜がある部分に乗り上げると反応しないため、地磁気を用いる
    // Vec2 dir_line_vec;
    // if(dir.dir_y > dir_threshold)  dir_line_vec.y += 1.0f;
    // if(dir.dir_y < dir_threshold)  dir_line_vec.y += -1.0f;

    // if(0 < dir.dir_z && dir.dir_z < 180.0f - dir_threshold)  dir_line_vec.x += 1.0f;
    // if(-180.0f + dir_threshold < dir.dir_z && dir.dir_z < 0) dir_line_vec.x -= 1.0f;

    // if(dir_line_vec.x != 0 && dir_line_vec.y != 0)
    //   motor.moveDir(degrees(atan2(dir_line_vec.y, dir_line_vec.x)), 100);
  }
  */

  motor.set(0, 0, 60.0f, 0);
  motor.avr();
  motor.write();

  delay(100);
}
