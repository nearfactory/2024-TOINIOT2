// old defender main program
// ATmega2560
// 2024-12-14

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

#include "StateMachine.hpp"

Ball ball;
Camera camera;
Dir dir;
Display display;
Kicker kicker;
Line line;
Motor motor;
Sub sub;
UI ui;


#define COMPILE 15
using namespace std;


State state = State::LineTrace;
State state_prev = State::LineTrace;
uint32_t state_begin = 0;


void setup() {
  Serial.begin(115200);
  Serial.println("2024-12-24 TOINIOT2 kansai def");

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

  // 白線に垂直に動くためのベクトル
  follow_dir = 0;
  if(line_dir > 0){
    if(line_dir-180 < ball.dir && ball.dir < line_dir){
      follow_dir = line_dir - 90.0;
    }else{
      follow_dir = line_dir + 90.0;
    }
  }else{
    if(line_dir < ball.dir && ball.dir < line_dir + 180){
      follow_dir = line_dir + 90.0;
    }else{
      follow_dir = line_dir - 90.0;
    }
  }

  if(ui.is_toggle){
    if(ui.buttonUp(0)) display.next();
    // for(auto b:ball.ball) Serial.printf("%d\t", b);
    // Serial.println();

    // variables
    display.addValiables("gain: "+to_string(gain), &gain);

    display.debug();
    display.draw();

    motor.set(0,0,0,0);
    is_display_on = true;

    motor.avr();
    motor.write();
    // delay(15);

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



  // 0.キックオフ
  if(state == State::KickOff){
    if(abs(dir.dir) > 90){
      motor.setDir(dir.dir, dir.dir_prev, dir.p_gain, dir.d_gain);
    }else{
      // state = State::BackToGoal_Weak;
      state = State::LineTrace;
    }
  }



  // 1.ライントレース
  if(state == State::LineTrace){
    Vec2 move_vec(0,0);


    // 白線に戻るベクトル
    move_vec.x += cos(radians(line.dir)) * distance;
    move_vec.y += sin(radians(line.dir)) * distance;


    // 白線と垂直に動くベクトル
    move_vec.x += cos(radians(follow_dir)) * gain;
    move_vec.y += sin(radians(follow_dir)) * gain;


    /*
    // ペナルティエリア内にボールがある場合に止める
    bool is_ball_penalty_area = false;
    if(is_ball_penalty_area){
      follow_power = 0;
    }
    */


    // 合成
    float move_dir = degrees(atan2(move_vec.y, move_vec.x));
    motor.moveDir(move_dir, 100);


    // ペナルティエリアの端で止まる
    if(abs(camera.def.dir)>26.0){
      if(abs(ball.dir) > 45.0){
        motor.moveDir(0,0);
      }
    }


    motor.setDirAdd(dir.dir, dir.dir_prev, dir.p_gain, dir.d_gain);


    // トリガー
    static bool     begin = false;
    static uint32_t timer = 0;
    bool is_dash = false;

    // -30~30かつ、<14400 の範囲にボールがある状態が2秒続けばキーパーダッシュにに移行
    if(abs(ball.dir)<30 && ball.distance < 13500){
      if(!begin){
        begin = true;
        timer = millis();
      }else{
        if(millis()-timer > 2000) is_dash = true;
      }
    }else{
      begin = false;
    }

    // →  2.キーパーダッシュ
    if(is_dash){
      state = State::KeeperDash;
    }
    // →  3.ゴール前に戻る(弱め)
    if(!line.on){
      state = State::BackToGoal_Weak;
    }

  }



  // 2.キーパーダッシュ
  else if(state == State::KeeperDash){
    motor.moveDir(0,0);
    if(state_elapsed > 2000){
      state = State::LineTrace;
    }
    /*
    float move_dir = 0;
    if(ball.is_hold){
      move_dir = camera.atk.dir;
    }else{
      move_dir = ball.dir * 1.5 - 4.2*(ball.dir - ball.dir_prev);
    }
    motor.moveDirFast(move_dir, 100);

    motor.setDirAdd(camera.atk.dir, camera.atk.dir_prev, dir.p_gain, dir.d_gain);


    bool is_line = line.on && abs(line.dir) < 90;
    bool is_stop = false;

    // → 4.ゴール前に戻る(強め)
    if(is_line || is_stop){
      state = State::BackToGoal_Strong;
    }
    */
  }
  


  // 3.ゴール前に戻る（弱め）
  else if(state == State::BackToGoal_Weak){
    // 最後のラインのベクトルへ移動
    motor.moveDir(line.dir, 70);
    motor.setDirAdd(dir.dir, dir.dir_prev, dir.p_gain, dir.d_gain);


    // トリガー
    static uint32_t is_not_line_begin = 0;
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
    motor.moveDir(camera.def.dir, 100);

    if(abs(dir.dir)>30){
      motor.setDir(dir.dir, dir.dir_prev, dir.p_gain, dir.d_gain);
    }else{
      motor.setDirAdd(dir.dir, dir.dir_prev, dir.p_gain, dir.d_gain);
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

  if(camera.def.is_visible){
    ui.buzzer(880.0f);
  }else{
    ui.buzzer(440.0f);
  }



  motor.avr();
  motor.write();

  kicker.write();
}
