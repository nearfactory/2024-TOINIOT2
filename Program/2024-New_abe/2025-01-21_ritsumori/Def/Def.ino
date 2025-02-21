// old defender main program
// ATmega2560
// 2024-12-14

#include <Wire.h>

#include "Ball.hpp"
#include "Camera.hpp"
#include "Dir.hpp"
#include "Display.hpp"
// #include "Kicker.hpp"
#include "Line.hpp"
#include "Motor.hpp"
#include "Sub.hpp"
#include "UI.hpp"

#include "StateMachine.hpp"

Ball ball;
Camera camera;
Dir dir;
Display display;
// Kicker kicker;
Line line;
Motor motor;
Sub sub;
UI ui;


#define COMPILE 15
using namespace std;


State state = State::LineTrace;
State state_prev = State::LineTrace;
uint32_t state_begin = 0;


float p_gain = 1.5;
float d_gain = 3.0;  // Test


void setup() {
  Serial.begin(115200);
  Serial.println("2024-12-24 TOINIOT2 kansai def");

  ball.begin();
  camera.begin();
  display.begin();
  dir.begin();
  // kicker.begin();
  line.begin(115200);
  motor.begin();
  sub.begin();
  ui.begin();
  

  // calibration
  uint8_t system=0, gyro=0, accel=0, mag=0;
  while(system<3 || gyro<3 || mag<3){
    dir.calibration(&system, &gyro, &accel, &mag);
    digitalWrite(LED_BUILTIN, HIGH);

    display.printd(8,8,to_string(COMPILE));

    display.printd(32,16,"system: "+to_string(system));
    display.printd(32,24,"gyro  : "+to_string(gyro));
    display.printd(32,32,"accel : "+to_string(accel));
    display.printd(32,40,"mag   : "+to_string(mag));

    display.printd(120, 8, "skip", ALIGN::RIGHT);
    ui.read();
    if(ui.buttonUp(1)) break;

    display.draw();
  }
  Serial.println("calibrated");

  // 攻め方向が設定されるまで待機
  display.printd(8,56,"set dir");
  display.draw();
  while(!ui.buttonUp(0)){ ui.read(); }
  display.draw();
  
  dir.setDefault();
}

float line_dir = 0;
float follow_dir   = 0;

float gain = 4.0f;

bool is_display_on = true;


void loop() {
  ball.read();
  camera.read();
  dir.read();
  line.read();
  sub.read();
  ui.read();



  if(ui.is_toggle){
    if(ui.buttonUp(0)) display.next();

    // variables
    // display.addValiables("p_gain :"+to_string(p_gain), &p_gain);
    // display.addValiables("d_gain :"+to_string(d_gain), &d_gain);
    // display.addValiables("diff :"+to_string(difference), &difference);

    display.debug();
    display.draw();

    state = State::BackToGoal_Strong;
    // state = State::KeeperDash;

    motor.set(0,0,0,0);
    is_display_on = true;

    motor.avr();
    motor.write();

    return;
  }
  // ディスプレイを消灯
  if(is_display_on){
    display.draw();
    is_display_on = false;
  }



  // ステートマシンを更新
  if(state != state_prev){
    state_begin = millis();
  }
  state_prev = state;
  uint32_t state_elapsed = millis() - state_begin;



  // 1.ライントレース
  if(state == State::LineTrace){
    // ボール追従の角度を算出
    float follow_dir = 0;
    if(line.dir < 0){
      if(line.dir < ball.dir && ball.dir < line.dir + 180)  follow_dir = line.dir + 90;
      else                                                  follow_dir = line.dir - 90;
    }else{
      if(line.dir - 180 < ball.dir && ball.dir < line.dir)  follow_dir = line.dir - 90;
      else                                                  follow_dir = line.dir + 90;
    }


    // ボールと白線の角度の差
    float b = ball.dir;
    if(b < -90) b += 180;
    if(90 < b)  b -= 180;

    float l = line.dir;
    if(l < -90) l += 180;
    if(90 < l)  l -= 180;

    float difference = abs(b-l);


    // ラインと同じ角度にボールがある場合に動かない
    if(difference < 15.0) {
      ui.buzzer(880.0f);
    }else{
      ui.buzzer(440.0f);
    }


    static bool is_block = false, is_side = false;
    static bool is_stop = false;
    
    if(abs(ball.dir - line.dir) < 10 /*|| abs(ball.dir - line.dir) > 170*/) {
      // is_block = true;
    }else{
      is_block = false;
    }

    // ラインが±90°付近のときに動かない
    if(is_block || is_side && ball.is_exist){
      // ロボットが敵陣向かって左
      if(0 < camera.def.dir){
        if(-45 < ball.dir)  is_side = false;
      }
      // 右
      else{
        if(ball.dir < 45)   is_side = false;
      }

      // 白線がズレた
      if(abs(line.dir) < 60 || 120 < abs(line.dir)) is_side = false;
    }else{
      if(80 < abs(line.dir) && abs(line.dir) < 100) is_side = true;
      if(camera.def.is_visible == false) is_side = true;
      if(abs(camera.def.dir) > 26) is_side = true;
    }


    if(is_block || is_side){
      motor.moveDir(0, 0);
    }else{
      // ベクトルを合成
      Vec2 move(0, 0);

      move.x += cos(radians(follow_dir));
      move.y += sin(radians(follow_dir));

      move.x += cos(radians(line.dir)) * line.distance;
      move.y += sin(radians(line.dir)) * line.distance;

      // ベクトルから角度に変換
      float move_dir = degrees(atan2(move.y, move.x));

      // 角度の差分から出力の強さに変換
      int min = 10, max = 30, max_power = 90;
      int power = max_power * (difference - min) / (max - min);
      if(power < 0) power = 0;
      if(max_power < power) power = max_power;

      motor.moveDir(move_dir, (uint8_t)power);
      motor.setDirAdd(dir.dir, dir.dir_prev, dir.p_gain, dir.d_gain);
    }


    // →  2.キーパーダッシュ
    if(ball.is_hold) state = State::KeeperDash;

    // 中立点のボールを押しだす
    // -30~30かつ、<14400 の範囲にボールがある状態が2秒続けばキーパーダッシュに移行
    static bool is_neutral = false;
    static uint32_t neutral_begin = millis();

    if(is_neutral == false){
      // キーパーダッシュに移行する条件
      if(abs(ball.dir) < 30 && ball.distance < 12000){
        is_neutral = true;
        neutral_begin = millis();
      }
    }else{
      if(millis() - neutral_begin > 2000){
        state = State::KeeperDash;
        is_neutral = true;
      }

      // ボールが動かされた判定
      if(abs(ball.dir) > 30 || ball.distance > 12000){
        is_neutral = false;
      }
    }

    

    // →  3.ゴール前に戻る(弱め)
    if(!line.on){
      state = State::BackToGoal_Weak;
    }

  }



  /*
  */
  // 2.キーパーダッシュ
  else if(state == State::KeeperDash){

    
    if(ball.is_hold == false){
      
      static float r = 8200;
      static float h = 10;

      float move_dir = 0;


      if(abs(ball.dir)<h){
        move_dir = ball.dir * p_gain - d_gain*(ball.dir - ball.dir_prev);
        h = 20;
      }
      // 円周上
      else if(ball.distance < r){
        float theta = 90 + (r-ball.distance) * 90 / r;
        move_dir = ball.dir + (ball.dir>0?theta:-theta);
        h = 10;
      }
      //接線
      else{
        float theta = degrees(asin(r / ball.distance));
        move_dir = ball.dir + (ball.dir>0?theta:-theta);
        h = 10;
      }
      motor.moveDir(move_dir, 60);
      motor.setDirAdd(dir.dir, dir.dir_prev, dir.p_gain, dir.d_gain);

    }else{
      motor.moveDir(camera.chance_dir, 80);
      motor.setDirAdd(dir.dir, dir.dir_prev, dir.p_gain, dir.d_gain);

      // if(camera.atk.h > 30){
      // }
        sub.kick();
        state = State::BackToGoal_Strong;
    }


    // 白線処理    
    if(line.on && state_elapsed > 200){

      float avoid_dir = 0;

      // 後ろ
      if(line.dir > 157.5 || line.dir < -157.5){
        if(abs(ball.dir) < 5) avoid_dir = 0;
        else if(ball.dir < 0) avoid_dir = 45;
        else                  avoid_dir = -45;
      }
      // 左後ろ
      else if(line.dir < -112.5){
        if(-135 < ball.dir && ball.dir < 45)  avoid_dir = 0;
        else                                  avoid_dir = 90;
      }
      // 左
      else if(line.dir < -67.5){
        if(-90 < ball.dir && ball.dir < 90)   avoid_dir = 45;
        else                                  avoid_dir = 135;
      }
      // 左前
      else if(line.dir < -22.5){
        if(-45 < ball.dir && ball.dir < 135)  avoid_dir = 90;
        else                                  avoid_dir = 180;
      }
      // 正面
      else if(line.dir < 22.5){
        if(abs(ball.dir) < 5) avoid_dir = 180;
        else if(ball.dir < 0) avoid_dir = 135;
        else                  avoid_dir = -135;
      }
      // 右前
      else if(line.dir < 67.5){
        if(-135 < ball.dir && ball.dir < 45)  avoid_dir = -90;
        else                                  avoid_dir = -180;
      }
      // 右
      else if(line.dir < 112.5){
        if(-90 < ball.dir && ball.dir < 90) avoid_dir = -45;
        else                                avoid_dir = -135;
      }
      // 右後ろ
      else{
        if(-45 < ball.dir && ball.dir < 135)  avoid_dir = 0;
        else                                  avoid_dir = -90;
      }

      motor.moveDir(avoid_dir, 90);
      motor.setDirAdd(dir.dir, dir.dir_prev, dir.p_gain, dir.d_gain);
    }


    // if(state_elapsed > 3000) state = State::LineTrace;

    // → 4.ゴール前に戻る(強め)
    // if(is_line || is_stop){
    //   state = State::BackToGoal_Strong;
    // }
  }
  


  // 3.ゴール前に戻る（弱め）
  else if(state == State::BackToGoal_Weak){
    // 最後のラインのベクトルへ移動
    motor.moveDir(line.dir, 90);
    motor.setDirAdd(dir.dir, dir.dir_prev, dir.p_gain, dir.d_gain);


    // トリガー
    // static uint32_t is_not_line_begin = 0;
    // if(line.on_prev == true && line.on == false){
    //   is_not_line_begin = millis();
    // }

    // →  1.ライントレース
    if(line.on){
      state = State::LineTrace;
    }
    // →  4.ゴール前に戻る(強め)
    // else if(millis()-is_not_line_begin > 2000){
    //   state = State::BackToGoal_Strong;
    // }
  }

  

  // 4.ゴール前に戻る（強め）
  else if(state == State::BackToGoal_Strong){

    if(abs(dir.dir)>30){
      motor.setDir(dir.dir, dir.dir_prev, dir.p_gain, dir.d_gain);
      // ui.buzzer(1.0f);
    }else if(state_elapsed > 1000){
      motor.moveDir(180 - camera.def.dir, 80);
      motor.setDirAdd(dir.dir, dir.dir_prev, dir.p_gain, dir.d_gain);
      // ui.buzzer(440.0f);
    }


    // →  1.ライントレース
    if(line.on){
      state = State::LineTrace;
    }
  }
  
  

  // 5.エラー
  else{
    state = State::LineTrace;
  }

  /*
  if(camera.def.is_visible){
    ui.buzzer(880.0f);
  }else{
    ui.buzzer(440.0f);
  }
  */



  motor.avr();
  motor.write();
}
