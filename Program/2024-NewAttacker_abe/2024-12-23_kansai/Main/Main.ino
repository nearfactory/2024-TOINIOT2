// new atacker kansai block program
// Teensy4.1
// 2024-12-24

#include <Wire.h>

#include "Ball.hpp"
#include "Camera.hpp"
#include "Dir.hpp"
#include "Display.hpp"
#include "Kicker.hpp"
#include "Line.hpp"
#include "Motor.hpp"
#include "Sub.hpp"
#include "UI.hpp"

Ball ball;
Camera camera;
Dir dir;
Display display;
Kicker kicker;
Line line;
Motor motor;
Sub sub;
UI ui;

using namespace std;

void setup() {
  Serial.begin(115200);
  Serial.println("2024-12-24 TOINIOT2 kansai");

  ball.begin();
  camera.begin();
  display.begin();
  dir.begin();
  kicker.begin();
  line.begin(115200);
  motor.begin();
  sub.begin();
  ui.begin();
  
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
    ui.read();
    if(ui.buttonUp(1)) break;

    display.draw();
  }

  // 攻め方向が設定されるまで待機
  display.printd(8,56,"set dir");
  display.draw();
  while(!ui.buttonUp(0)){ ui.read(); }
  
  dir.setDefault();
}


bool is_display_on = true;

// 周り込み
float h = 45;       // ヒステリシス
float r = 14400.0f; // 回り込みの半径
float p_gain = 2.0f;
float d_gain = 3.5f;

// ボールの運搬
bool      shoot = false;
uint32_t  shoot_timer = 0;
float     shoot_dir_begin = 0;

// ボールのキック動作
bool     kick = false;
uint32_t kick_timer = 0;


void loop() {
  // delayなしで3(ms)

  ball.read();
  camera.read();
  dir.read();
  line.read();
  sub.read();
  ui.read();


  if(digitalRead(ui.TOGGLE_PIN)){
    if(ui.buttonUp(0)) display.next();

    // display.addValiables("p_gain :"+to_string(p_gain), &p_gain);
    // display.addValiables("d_gain :"+to_string(d_gain), &d_gain);

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




    // シュート
    // if(shoot){
    if(false){
      motor.moveDirFast(-camera.goal_dir* 1.2, 100);

      // シュート終了
      if(ball.not_hold_time > 200) shoot = false;

      // ゴール前がガラ空きの場合にボールをキックする
      if(camera.atk_num == 1 && 80 < camera.atk_w && camera.atk_w < 120 && millis() - shoot_timer > 200){
        kick = true;
        kick_timer = millis();
      }

      // キック動作
      if(kick){
        // ゴールに向ける
        if(millis() - kick_timer < 100){
          float dir_power = camera.goal_dir * 6.0;
          motor.add(dir_power, dir_power, dir_power, dir_power);
        }
        // キック
        kicker.kick();

        // キック終了
        if(100 < millis() - kick_timer){
          shoot = false;
          kick = false;
        }

      }
        

    }
    // 回り込み(方法4) https://yuta.techblog.jp/archives/40889399.html
    else{
      float move_dir = 0;

      // PD
      // static float goal_dir = 0;

      // float wrap_around_dir = ball.dir + goal_dir;
      // float wrap_around_dir = ball.dir;

      // if(abs(wrap_around_dir)<h){
      //   move_dir = wrap_around_dir * p_gain - d_gain*(wrap_around_dir - ball.dir_prev);
      //   h = 45.0;
      // }
      // // 円周上
      // else if(ball.distance < r){
      //   float theta = 90 + (r-ball.distance) * 90 / r;
      //   move_dir = wrap_around_dir + (wrap_around_dir>0?theta:-theta);
      //   h = 30.0;

      //   float threshold = 20.0;
      //   if(camera.prev_goal_dir < -threshold){
      //     goal_dir = threshold;
      //   }else if(-threshold <= camera.prev_goal_dir && camera.prev_goal_dir <= threshold){
      //     goal_dir = 0;
      //   }else if(threshold < camera.prev_goal_dir){
      //     goal_dir = -threshold;
      //   }

      // }
      // //接線
      // else{
      //   float theta = degrees(atan2(r, ball.distance));
      //   move_dir = wrap_around_dir + (wrap_around_dir>0?theta:-theta);
      //   h = 30.0;
      // }

      if(abs(ball.dir)<h){
        move_dir = ball.dir * p_gain - d_gain*(ball.dir - ball.dir_prev);
        h = 45;

        // シュート動作に入る
        if(ball.is_hold){
          shoot = true;
          shoot_dir_begin = camera.goal_dir;
          shoot_timer = millis();
        } 
      }
      // 円周上
      else if(ball.distance < r){
        float theta = 90 + (r-ball.distance) * 90 / r;
        move_dir = ball.dir + (ball.dir>0?theta:-theta);
        h = 30;
      }
      //接線
      else{
        float theta = degrees(atan2(r, ball.distance));
        move_dir = ball.dir + (ball.dir>0?theta:-theta);
        h = 30;
      }

      motor.moveDir(move_dir, 100);
    }


    if(ball.is_hold){
      while(!ui.buttonUp(1)){
        ui.read();
        motor.moveDir(0, 0);
        motor.avr();
        motor.write();
        delay(3);
      }
    } 



    // ボールが見えない場合に後ろに下がる (デバッグ段階では手前に)
    if(!ball.is_exist){
      motor.moveDir(0, 60);
    }

    


    // 白線避け
    if(line.on){
      motor.moveDir(line.dir+180, 100);
    }


    // 姿勢制御

    float p_gain = 0.64f;
    float d_gain = 0.45f;
    // float dir_power = (dir.dir+camera.goal_dir) * p_gain - (dir.prev_dir - dir.dir) * d_gain;
    float dir_power = 0;
    if(camera.atk_num && abs(dir.dir) < 90){
      dir_power = (camera.goal_dir) * p_gain - (dir.prev_dir - dir.dir) * d_gain;
    }else{
      dir_power = (dir.dir) * p_gain - (dir.prev_dir - dir.dir) * d_gain;
    }

    if(abs(dir.dir) > 45) {
      // 故障復帰
      motor.set(dir_power, dir_power, dir_power, dir_power);
    }else{
      // 姿勢制御
      motor.add(dir_power, dir_power, dir_power, dir_power);
    }

    if(ball.is_hold){
      ui.buzzer(880.0f);
    }else{
      // ui.buzzer(440.0f);
      digitalWrite(ui.BZ_PIN, 0);
    }

  }


  motor.avr();
  motor.write();
  
  kicker.write();
}