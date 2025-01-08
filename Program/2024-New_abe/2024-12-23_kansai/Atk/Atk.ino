// new atacker kansai block program
// Teensy4.1
// 2024-01-05

#include <Wire.h>

#include "Ball.hpp"
#include "Camera.hpp"
#include "Dir.hpp"
#include "Display.hpp"
#include "Line.hpp"
#include "Motor.hpp"
#include "Sub.hpp"
#include "UI.hpp"

Ball ball;
Camera camera;
Dir dir;
Display display;
Line line;
Motor motor;
Sub sub;
UI ui;

// #include "Kicker.hpp"
// Kicker kicker;

using namespace std;



bool is_display_on = true;

// 周り込み
float h = 45;       // ヒステリシス
// float r = 14400.0; // 回り込みの半径
float r = 7000.0; // 回り込みの半径
float p_gain = 1.5;
float d_gain = 4.2;


// ステートマシン
State state = State::KickOff;
State state_prev = State::KickOff;
uint32_t state_begin = 0;

// ToDo: 白線から出ない速度の調整・半分超えた場合の処理
float speed_normal = 100.0;



void setup() {
  Serial.begin(115200);
  Serial.println("2024-12-24 TOINIOT2 kansai");

  ball.begin();
  camera.begin();
  display.begin();
  dir.begin();
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
  display.draw();
  
  dir.setDefault();
}



void loop() {
  // delayなしで約3(ms)

  ball.read();
  camera.read();
  dir.read();
  line.read();
  sub.read();
  ui.read();



  // ディスプレイ
  if(ui.is_toggle){
    Serial.println("display");
    if(ui.buttonUp(0)) display.next();

    // display.addValiables("p_gain :"+to_string(p_gain), &p_gain);
    // display.addValiables("d_gain :"+to_string(d_gain), &d_gain);
    display.addValiables("speed_normal :"+to_string(speed_normal), &speed_normal);

    display.debug();
    display.draw();
    is_display_on = true;

    state = State::Follow;
    state_begin = millis();

    motor.set(0,0,0,0);

    motor.avr();
    motor.write();
    delay(10);


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



  // ok?: キックオフ(まっすぐ進めない時がある)
  if(state == State::KickOff){
    motor.moveDirFast(ball.dir, 100);
    if(line.on) motor.moveDirFast(line.dir+180, 100);
    motor.setDirAdd(dir.dir, dir.dir_prev, dir.p_gain, dir.d_gain);


    // 後ろ向き -> 故障復帰
    if(abs(dir.dir) > 90){
      state = State::Damaged;
    }

    // 1秒経過 -> 回り込み
    if(state_elapsed > 1000){
      state = State::Follow;
    }

  }



  // ok: 故障復帰
  else if(state == State::Damaged){
    motor.setDir(dir.dir, dir.dir_prev, dir.p_gain, dir.d_gain);
    if(line.on) motor.moveDir(line.dir+180, 100);


    // 45度以内 -> 回り込み
    if(abs(dir.dir) < 45){
      state = State::Follow;
    }
  }



  // ok: 回り込み
  else if(state == State::Follow){
    float move_dir = 0;

    // PD
    if(abs(ball.dir)<h){
      move_dir = ball.dir * p_gain - d_gain*(ball.dir -     ball.dir_prev);
      h = 45;
    }
    // 円周上
    else if(ball.distance < r){
      float theta = 90 + (r-ball.distance) * 90 / r;
      move_dir = ball.dir + (ball.dir>0?theta:-theta);
      h = 20;
    }
    //接線
    else{
      float theta = degrees(atan2(r, ball.distance));
      move_dir = ball.dir + (ball.dir>0?theta:-theta);
      h = 20;
    }

    motor.moveDir(move_dir, speed_normal);
    if(line.on) motor.moveDir(line.dir+180, 100);
    motor.setDirAdd(dir.dir, dir.dir_prev, dir.p_gain, dir.d_gain);

   
    // ボールを保持 -> ゴールに向かう
    if(ball.is_hold){
      state = State::Dribble;
    }

    if(!ball.is_exist) state = State::NoBall;

  }



  // ToDo: ゴールに向かう
  else if(state == State::Dribble){
    // キーパーのいない方のゴールの角を狙う
    // 保持したまま動けるのは±30°
    
    // 攻める角度の決定

    static bool is_decided = false;
    static int  type = 0;

    // 方式の決定
    if(!is_decided){
      if(sub.ready){
        type = 0;
      }else{
        type = 1;
      }
    }

    // キッカー
    if(type == 0){
      static bool     kick_begin = false;
      static uint32_t kick_timer = 0;

      // 直進
      if(kick_begin == false){
        motor.moveDirFast(0, 100);
        if(line.on) motor.moveDirFast(line.dir+180, 100);
        motor.setDirAdd(camera.atk.dir, camera.atk.dir_prev, dir.p_gain, dir.d_gain);

        // キックに移行
        if(camera.atk.h > 26 && millis()-kick_timer > 250 && abs(camera.atk.dir) < 10.0){
          kick_begin = true;
          kick_timer = millis();
        }
      }

      // キック
      else{
        // 0.2秒間キック
        if(millis()-kick_timer < 200){
          motor.moveDirFast(0, 100);
          if(line.on) motor.moveDirFast(line.dir+180, 100);
          sub.kick();
        }else{
          kick_begin = false;
          state = State::Follow;
        }
      }

    }


    // ねじる
    else if(type == 1){
      static bool     nejiri_begin = false;
      static uint32_t nejiri_timer = 0;

      // 直進する
      if(nejiri_begin == false){
        motor.moveDirFast(0, 100);
        if(line.on) motor.moveDirFast(line.dir+180, 100);
        motor.setDirAdd(dir.dir, dir.dir_prev, dir.p_gain, dir.d_gain);

        // ねじりに移行
        if(camera.atk.h > 24 && millis()-nejiri_timer > 500){
          nejiri_begin = true;
          nejiri_timer = millis();
        }
      }

      // ねじる
      else{
        // 0.4秒ねじったら回り込みに戻る
        if(millis()-nejiri_timer < 400){
          motor.moveDirFast(0, 100);
          if(line.on) motor.moveDirFast(line.dir+180, 100);
          motor.setDirAdd(camera.atk.dir, camera.atk.dir_prev, 6.0f, 0.0f);
        }else{
          nejiri_begin = false;
          state = State::Follow;
        }

      }

    }


    // ボールを保持していない -> 回り込みなおす
    if(!ball.is_hold){
      state = State::Follow;
    }

    // ロボットが動かない -> 押し合い

    // ボールなし -> ボールなし
    if(!ball.is_exist) state = State::NoBall;


    // キーパーが目の前にいる -> キーパー避け
  }



  // シュート(一旦無視)
  else if(state == State::Shoot){
    static int  type = 0;
    static bool is_decided = false;

    // シュート方式の決定
    if(is_decided == false){
      // ゴールに向いている倍位はキッカー作動
      if(abs(camera.atk.dir) < 30){
        type = 0;
      }
      
      // ゴールを向けていない場合は機体をねじる
      else{
        type = 1;
      }
    }


    // キッカー使用
    // チャージされていない場合突進
    if(type == 0){
      motor.moveDirFast(camera.atk.dir, 100);
      if(state_elapsed > 50) sub.kick();
    }

    // 機体のねじり  
    else if(type == 1){
      motor.moveDirFast(camera.atk.dir,100);
      motor.setDirAdd(camera.atk.dir, 0, 6.0, 0);
    }


    // キック終了(100ms) -> 回り込み
    if(state_elapsed > 400){
      state = State::Follow;
    }

  }



  // キーパーをどかす
  else if(state == State::AvoidKeeper){
    // ゴール左側に向かいながら右を狙う
    float move_limit = 0;   // ボールを保持したまま平行移動できる限界の角度
    float goal_dir = camera.atk.dir;  // 狙う角度(右)
    static float goal_dir_prev = 0;

    motor.moveDir(goal_dir-move_limit, 100);
    motor.setDirAdd(goal_dir, goal_dir_prev, dir.p_gain, dir.d_gain);

    goal_dir_prev = goal_dir;


    // センターサークルを超えた -> シュート
    if(camera.atk.h > 40){
      state = State::Shoot;
    }

    // キーパーがどいた -> ドリブル
  }



  // 押し合い
  else if(state == State::Pushing){
    // 左に押した後、右に切り返す
    if(state_elapsed < 2500){
      motor.moveDirFast(-22.5, 100);
      motor.setDirAdd(dir.dir+10, dir.dir_prev, dir.p_gain, dir.d_gain);
    }
    else{
      motor.moveDirFast(22.5,100);
      motor.setDirAdd(dir.dir-10, dir.dir_prev, dir.p_gain, dir.d_gain);
    }


    // ボールを奪えた -> シュート
  }



  // ok: ボールが取り上げられた
  else if(state == State::NoBall){
    // 真ん中くらいまで下がる
    static uint32_t timer = 0;
    static bool timer_begin = false;
    if(camera.atk.h < 26 && timer_begin == false){
      timer = millis();
      timer_begin = true;
    }

    if(timer_begin == false || millis()-timer < 500){
      motor.moveDir(180 - dir.dir, speed_normal);
      if(line.on) motor.moveDir(line.dir+180, 100);
      motor.setDirAdd(dir.dir, dir.dir_prev, dir.p_gain, dir.d_gain);
    }else{
      motor.moveDir(0, 0);
      if(line.on) motor.moveDir(line.dir+180, speed_normal);
    }


    // 見えた -> 回り込み
    if(ball.is_exist){
      timer_begin = false;
      state = State::Follow;
    }
  }



  // 中立点からの回り込み・シュート
  else if(state == State::Neutral){
    // ゴールの方に45度傾ける
    motor.moveDir(ball.dir, 100);
  }



  // テスト
  else if(state == State::Test){
    // motor.setDir(camera.chance_dir, camera.chance_dir_prev, dir.p_gain * 3, dir.d_gain * 3);
  }
  else{
    state = State::Follow;
  }


  
  // if(state == State::Dribble){
  if(ball.is_hold){
    ui.buzzer(880.0f);
  }else{
    ui.buzzer(440.0f);
  }





  motor.avr();
  motor.write();
}