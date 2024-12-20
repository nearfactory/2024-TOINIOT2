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
#include "Kicker.hpp"
#include "UI.hpp"

Ball ball;
Camera camera;
Dir dir;
Display display;
Kicker kicker;
Line line;
Motor motor;
UI ui;

using namespace std;

void setup() {
  Serial.begin(115200);
  Serial.println("begin");
  Serial7.begin(115200);

  ball.begin();
  // camera.begin();
  display.begin();
  dir.begin();
  kicker.begin();
  line.begin(115200);
  motor.begin();
  
  // calibration
  uint8_t system=0, gyro=0, accel=0, mag=0;
  while(system<3 || gyro<3 || mag<3){
    dir.calibration(&system, &gyro, &accel, &mag);
    digitalWrite(LED_BUILTIN, HIGH);

    display.printd(32,16,"system: "+to_string(system));
    display.printd(32,24,"gyro  : "+to_string(gyro));
    display.printd(32,32,"accel : "+to_string(accel));
    display.printd(32,40,"mag   : "+to_string(mag));

    display.printd(120, 8, "skip", ALIGN::RIGHT);
    display.draw();

    ui.read();
    if(ui.buttonUp(1)) break;

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

float p1 = 10.5;
float line_power = 12.0;

float dir_threshold = 2.0f;
float dir_p_gain = 0.4f;

float avoid_dir = 0;
bool  prev_on = false;

uint32_t left_timer = 0;
uint32_t right_timer = 0;


void loop() {
  ball.read();

  // auto t = millis();
  // camera.read();
  // Serial.printf("camera:%d ", millis()-t);

  dir.read();
  line.read();

  if(digitalRead(ui.TOGGLE_PIN)){
    // 25(ms)
    ui.read();
    if(ui.buttonUp(0)) display.next();

    // display.addValiables("pow   :"+to_string(p1), &p1);
    // display.addValiables("pow_li:"+to_string(line_power), &line_power);
    display.addValiables("p_gain :"+to_string(p_gain), &p_gain);
    // display.addValiables("dir_p :"+to_string(dir_p_gain), &dir_p_gain);
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


    if(ball.hold_time > 80){
    // if(false){
      float power = 100.0;
      motor.set(-power, -power, power, power);
      
      // キッカー
      if(ball.hold_time > 150){
        kicker.kick();
      }

    }else{
      // 回り込み(方法4) https://yuta.techblog.jp/archives/40889399.html
      float theta = 0;
      float move_dir = 0;
      if(abs(ball.dir)<h){
        // PD
        move_dir = ball.dir * p_gain - d_gain*(ball.dir - ball.dir_prev);
        h = 45;
        
        // if(ball.hold_time > 300){
        //   kicker.kick();
        // }
        // 円周
      }else if(ball.distance < r){
        theta = 90 + (r-ball.distance) * 90 / r;
        move_dir = ball.dir + (ball.dir>0?theta:-theta);
        h = 45;
      }else{
        //接線
        theta = degrees(atan2(r, ball.distance));
        move_dir = ball.dir + (ball.dir>0?theta:-theta);
        h = 45;
      }
      motor.moveDir(move_dir, p1*10.0);
    }



    // ボールが見えない場合に後ろに下がる (デバッグ段階では手前に)
    if(!ball.is_exist){
      motor.moveDir(0, 60);
    }


    // 姿勢制御
    float dir_power = 0;

    // 機体をゴールに向ける
    if(ball.is_hold){
      // float goal_dir = (camera.x - 160);
      // dir_power = (dir.dir - goal_dir) * dir_p_gain;
      // motor.add(dir_power, dir_power, dir_power, dir_power);

      dir_power = dir.dir * dir_p_gain;
      motor.addRaw(dir_power, dir_power, dir_power, dir_power);
    }else if(abs(dir.dir) > 30){
      float p_gain = 0.67f;
      float d_gain = 0.45f;
      dir_power = dir.dir * p_gain - (dir.prev_dir - dir.dir) * d_gain;
      motor.set(dir_power, dir_power, dir_power, dir_power);
    }else{
      dir_power = dir.dir * dir_p_gain;
      float p_gain = 0.67f;
      float d_gain = 0.45f;
      dir_power = dir.dir * p_gain - (dir.prev_dir - dir.dir) * d_gain;
      motor.addRaw(dir_power, dir_power, dir_power, dir_power);
    }



    // 白線避け
    if(line.on){
      motor.moveDir(line.dir+180, line_power*10.0);
      // motor.moveDir(0,0);
    }

    if(line.left){
      left_timer = millis();
    }
    if(millis()-left_timer < 160){
      motor.moveDir(90, line_power*10.0);
    }

    if(line.right){
      right_timer = millis();
    }
    if(millis()-right_timer < 160){
      motor.moveDir(-90, line_power*10.0);
    }
    // if(line.left){
    //   while(!ui.buttonUp(0)){
    //     ui.read();
    //     motor.moveDir(0, 0);
    //     motor.avr();
    //     motor.write();
    //     delay(50);
    //   }
    // }
    // prev_on = line.on;
    



    // コート橋の傾斜がある部分に乗り上げるとラインセンサが反応しないため、地磁気を用いる
    // Vec2 dir_line_vec;
    // if(dir.dir_y > dir_threshold)  dir_line_vec.y += 1.0f;
    // if(dir.dir_y < dir_threshold)  dir_line_vec.y += -1.0f;

    // if(0 < dir.dir_z && dir.dir_z < 180.0f - dir_threshold)  dir_line_vec.x += 1.0f;
    // if(-180.0f + dir_threshold < dir.dir_z && dir.dir_z < 0) dir_line_vec.x -= 1.0f;

    // if(dir_line_vec.x != 0 && dir_line_vec.y != 0)
    //   motor.moveDir(degrees(atan2(dir_line_vec.y, dir_line_vec.x)), 100);
  }

  if(ball.is_hold){
    digitalWrite(LED_BUILTIN, HIGH);
    ui.buzzer(880.0f);
  }else{
    digitalWrite(LED_BUILTIN, LOW);
    ui.buzzer(440.0f);
  }

  motor.avr();
  motor.write();
  
  kicker.write();

  // delay(10);
  // delayなしで3(ms)

  /*
  ToDo:
    done: 0.dir<-30, 30<dir の範囲では姿勢制御のみを行うようにする
    done: 1.確実に白線を避ける
      idea:
      ・ng: ラインセンサのしきい値を下げる
      ・done: 移動平均のサンプル数を減らす・ループの速度を上げる
      ・ng: 地磁気を用いる
      ・? : カメラを用いた自己位置推測
      ・ng: 反応しないセンサを補完する
    done: 2.キッカー基板が使えないため、現状の情報でボールを保持したか否か判断するアルゴリズムを書く
      ・ng: ボールとの距離を用いる
      ・ng: 距離に対する角度のブレ
      ・done: ヒステリシス
    3.ボールを保持した場合の動作を書く
      ・done: キッカーがないため突進する速度を上げる
      ・敵を避けてプッシングを取られないようにする
    4.両側のゴールに対して攻められるように対応する
  */

}