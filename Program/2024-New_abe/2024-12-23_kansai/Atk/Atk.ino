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
float h = 45;         // ヒステリシス
float r = 13000.0;    // 回り込みの半径
float p_gain = 1.5;
float d_gain = 8.0;

float offset = 1.0;


// ステートマシン
State state      = State::KickOff;
State state_prev = State::KickOff;
uint32_t state_begin = 0;



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
    display.printd(8,8,to_string(COMPILE));
    
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
    // Serial.println("display");
    if(ui.buttonUp(0)) display.next();

    display.addValiables("p_gain :"+to_string(p_gain), &p_gain);
    display.addValiables("d_gain :"+to_string(d_gain), &d_gain);
    display.addValiables("offset: "+to_string(offset), &offset);
    display.addValiables("speed_normal :"+to_string(speed_normal), &speed_normal);

    display.debug();
    display.draw();
    is_display_on = true;

    state = State::KickOff;
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
  int line_flag = 0;



  // ok: キックオフ
  if(state == State::KickOff){
    if(line.on) line_flag = 2;
    motor.moveDirFast(ball.dir, 100);
    motor.setDirAdd(dir.dir, dir.dir_prev, dir.p_gain, dir.d_gain);


    // 後ろ向き -> 故障復帰
    if(abs(dir.dir) > 90){
      state = State::Damaged;
    }

    // 保持 -> ドリブル
    if(ball.is_hold){
      state = State::Dribble;
    }

    // 0.5秒経過 -> 回り込み
    if(state_elapsed > 500){
      state = State::Follow;
    }

  }



  // ok: 故障復帰
  else if(state == State::Damaged){
    motor.setDir(dir.dir, dir.dir_prev, dir.p_gain, dir.d_gain);
    if(line.on) line_flag = 1;


    // 45度以内 -> 回り込み
    if(abs(dir.dir) < 45){
      state = State::Follow;
    }
  }



  // ToDo: 回り込み
  else if(state == State::Follow){
    float move_dir = 0;

    // 直進
    // if(abs(ball.dir)<5){
    //   move_dir = ball.dir;
    // }

    // PD
    if(abs(ball.dir)<h){
      move_dir = ball.dir * p_gain - d_gain*(ball.dir - ball.dir_prev);
      h = 15;
    }
    // 円周上
    else if(ball.distance < r){
      float theta = 90 + (r-ball.distance) * 90 / r;
      move_dir = ball.dir + (ball.dir>0?theta:-theta);
      h = 15;
    }
    //接線
    else{
      float theta = degrees(atan2(r, ball.distance));
      move_dir = ball.dir + (ball.dir>0?theta:-theta);
      h = 15;
    }

    if(line.on) line_flag = 1;
    motor.moveDir(move_dir, 95); 

    // ゴールに向ける
    float face = 0;
    static int dir_type = 0;
    switch(dir_type){
      case 0:
        r = 12500.0;
        // ゴールに近い場合のみ
        if(camera.atk.h > 25){
          if(camera.atk.dir < -23){
            dir_type = 1;
          }else if(camera.atk.dir > 23){
            dir_type = 2;
          }
        }
        face = 0;
        break;
      case 1:
        r = 12500.0;
        if(camera.atk.dir > 10){
          dir_type = 0;
        }
        face = -23;
        break;
      case 2:
        r = 12500.0;
        if(camera.atk.dir < -10){
          dir_type = 0;
        }
        face = 20;
        break;
    }
    motor.setDirAdd(dir.dir + face, dir.dir_prev, dir.p_gain, dir.d_gain);

   
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
      // キッカーok かつ 姿勢制御が間に合う
      if(sub.ready && abs(camera.atk.dir) < 20.0){
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
        if(line.on) line_flag = 2;
        motor.moveDirFast(0, 100);
        motor.setDirAdd(camera.atk.dir, camera.atk.dir_prev, dir.p_gain, dir.d_gain);

        // キックに移行
        if(camera.atk.h > 26 && state_elapsed > 250 && abs(camera.atk.dir) < 10.0){
          kick_begin = true;
          kick_timer = millis();
        }
      }

      // キック
      else{
        // 0.2秒間キック
        if(millis()-kick_timer < 200){
          if(line.on) line_flag = 2;
          motor.moveDirFast(0, 100);
        }else{
          sub.kick();
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
        if(line.on) line_flag = 2;
        motor.moveDirFast(0, 100);
        motor.setDirAdd(dir.dir, dir.dir_prev, dir.p_gain, dir.d_gain);

        // ねじりに移行
        if(camera.atk.h > 24){
          nejiri_begin = true;
          nejiri_timer = millis();
        }
      }

      // ねじる
      else{
        // 0.4秒ねじったら回り込みに戻る
        if(millis()-nejiri_timer < 400){
          if(line.on) line_flag = 2;
          motor.moveDirFast(0, 100);
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

    // ロボットが動かない(≒このステートで5秒経過 -> 押し合い
    if(state_elapsed > 5000){
      state = State::Pushing;
    }

    // ボールなし -> ボールなし
    if(!ball.is_exist) state = State::NoBall;


    // キーパーが目の前にいる -> キーパー避け
  }



  // キーパーをどかす
  /*
  else if(state == State::AvoidKeeper){
    // ゴール左側に向かいながら右を狙う
    static bool is_decided = false;
    static float atk_dir = 0;

    if(!is_decided){
      if(camera.atk.dir < 0)  atk_dir = 30;
      else                    atk_dir = -30;
    }

    
    motor.moveDir(-dir.dir, 100);
    motor.setDirAdd(dir.dir-atk_dir, dir.dir_prev, dir.p_gain, dir.d_gain);


    if(!ball.is_hold){
      state = State::Follow;
    }


    // ゴールを中心に捉えた -> シュート
    if(camera.atk.x1-140 < 0 && 0 < camera.atk.x2-140){
      sub.kick();
      state = State::Follow;
    }

    // キーパーがどいた -> ドリブル
  }
  */



  // 押し合い
  else if(state == State::Pushing){
    // 左に押した後、右に切り返す
    if(state_elapsed < 2000){
      motor.moveDirFast(-22.5, 100);
      motor.setDirAdd(dir.dir+10, dir.dir_prev, dir.p_gain, dir.d_gain);
    }
    else{
      motor.moveDirFast(22.5,100);
      motor.setDirAdd(dir.dir-10, dir.dir_prev, dir.p_gain, dir.d_gain);
    }


    // 4秒経過 -> ドリブルに戻る
    if(state_elapsed > 4000){
      state = State::Dribble;
    }

    // ボールを奪えた -> シュート
  }



  // ok: ボールが取り上げられた
  else if(state == State::NoBall){
    // 真ん中くらいまで下がる
    // if(state_elapsed < 1000){
    //   if(line.on) line_flag = 1;
    //   motor.moveDir(180 - dir.dir, speed_normal*10.0);
    //   motor.setDirAdd(dir.dir, dir.dir_prev, dir.p_gain, dir.d_gain);
    // }else{
    //   if(line.on) line_flag = 1;
    //   motor.moveDir(0, 0);
    // }

    motor.setDir(dir.dir, dir.dir_prev, dir.p_gain, dir.d_gain);


    // 後ろに見えた -> 逆の回り込み
    
    // 前に見えた -> 回り込み
    if(ball.is_exist){
      // timer_begin = false;
      state = State::Follow;
    }
  }



  /*
  // ボールを持って後ろに下がる
  else if(state == State::Back){
    // PD
    if(abs(ball.dir)<h){
      move_dir = follow_dir * p_gain - d_gain*(follow_dir -     follow_dir_prev);
      // move_dir = follow_dir;
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
    
    // 半分超えたら回り込み直し
    if(camera.atk.h > 24){
      state = State::Follow;
    }
  }
  */



  /*
  // テスト
  else if(state == State::Test){
    // motor.setDir(camera.chance_dir, camera.chance_dir_prev, dir.p_gain * 3, dir.d_gain * 3);
  }
  else{
    state = State::Follow;
  }
  */



  // 白線処理
  static uint32_t timer = 0;

  // 速い場合に0.2秒間戻る
  if(millis()-timer < 200){
    motor.moveDirFast(line.dir+180, 100);
  }else{
    switch(line_flag){
    case 1:
      motor.moveDirFast(line.dir+180, 100);
      break;
    case 2:
      timer = millis();
      break;
    }

  }



  motor.avr();
  motor.write();
}