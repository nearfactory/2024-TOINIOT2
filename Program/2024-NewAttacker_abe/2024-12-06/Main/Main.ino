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
Camera camera;
Dir dir;
Display display;
Line line;
Motor motor;
UI ui;

using namespace std;

void setup() {
  Serial.begin(115200);
  Serial.println("begin");
  Serial7.begin(115200);

  ball.begin();
  camera.begin();
  display.begin();
  dir.begin();
  line.begin(115200);
  motor.begin();
  
  // calibration
  uint8_t system=0, gyro=0, accel=0, mag=0;
  system = 3; gyro = 3; mag = 3;
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
}

bool is_display_on = true;

int h = 45; // ヒステリシス
float r = 14400.0f;  // 回り込み時の半径
float p_gain = 1.5f;
float d_gain = 3.5f;
float* gain_select = &p_gain;

float p1 = 8.5f;

float dir_threshold = 2.0f;
float dir_p_gain = 1.0f;


void loop() {
  ball.read();

  auto t = millis();
  camera.read();
  Serial.printf("camera:%d ", millis()-t);

  dir.read();
  line.read();

  if(digitalRead(ui.TOGGLE_PIN)){

    ui.read();
    if(ui.buttonUp(0)) display.next();

    display.addValiables("power :"+to_string(p1), &p1);
    display.addValiables("dir_p :"+to_string(dir_p_gain), &dir_p_gain);
    // display.addValiables("dir_th:"+to_string(dir_threshold), &dir_threshold);
    // display.addValiables("dir y :"+to_string(dir.dir_y), &dir.dir_y);
    // display.addValiables("dir z :"+to_string(dir.dir_z), &dir.dir_z);

    display.debug();
    display.draw();

    motor.set(0,0,0,0);
    is_display_on = true;

  }else{
    // ディスプレイを消灯
    if(is_display_on){
      display.draw();
      is_display_on = false;
    }


    if(ball.is_holding){
      float power = 100.0;
      motor.set(power, power, -power, -power);
    }else{
      // 回り込み(方法4) https://yuta.techblog.jp/archives/40889399.html
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
    }



    // ボールが見えない場合に後ろに下がる (デバッグ段階では手前に)
    if(!ball.is_exist){
      motor.moveDir(0, 60);
    }

    

    // 姿勢制御
    float dir_power = 0;

    // 機体をゴールに向ける
    if(ball.is_hold){
      float goal_dir = (camera.x - 160);
      dir_power = (dir.dir - goal_dir) * dir_p_gain;
    }else if(abs(dir.dir) > 30){
      float p_gain = 0.67f;
      float d_gain = 0.45f;
      dir_power = dir.dir * p_gain - (dir.prev_dir - dir.dir) * d_gain;
    }else{
      dir_power = dir.dir * dir_p_gain;
    }
    motor.addRaw(dir_power, dir_power, dir_power, dir_power);



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

  motor.avr();
  motor.write();

  delay(10);

  /*
  ToDo:
    done: 0.dir<-30, 30<dir の範囲では姿勢制御のみを行うようにする
    1.確実に白線を避ける
      idea:
      ・移動平均のサンプル数を減らす・ループの速度を上げる
      ・地磁気を用いる
      ・カメラを用いた自己位置推測
      ・反応しないセンサを補完する
    2.キッカー基盤が使えないため、現状の情報でボールを保持したか否か判断するアルゴリズムを書く
      ・ボールとの距離を用いる
      ・距離に対する角度のブレ
      ・ヒステリシス
    3.ボールを保持した場合の動作を書く
      done: ・キッカーがないため突進する速度を上げる
      ・敵を避けてプッシングをトラれないようにする
    4.両側のゴールに対して攻められるように対応する
  */

}