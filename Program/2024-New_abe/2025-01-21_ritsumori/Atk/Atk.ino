// new atacker ritsumori
// Teensy4.1
// 2024-02-23

#include <cmath>

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
float h = 20;         // ヒステリシス
float r = 11600.0;    // 回り込みの半径
float p_gain = 1.5;
float d_gain = 3.0;  // Test

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

  Serial.println("start");
}



void loop() {
  // delayなしで約3(ms)

  ball.read();
  camera.read();
  dir.read();
  line.read();
  sub.read();
  ui.read();

static float _push_move = 2.0, _push_dir = 5.0;
  float push_move = (int)(_push_move * 10.0f), push_dir = (int)(_push_dir * 10.0f);

  // ディスプレイ
  if(ui.is_toggle){
    Serial.println("display");
    camera.lock = false;
    if(ui.buttonUp(0)) display.next();

    display.addValiables("p_gain :"+to_string(p_gain), &p_gain);
    display.addValiables("d_gain :"+to_string(d_gain), &d_gain);
    // display.addValiables("offset: "+to_string(offset), &offset);
    // display.addValiables("x: "+to_string(dir.x), &dir.x);
    // display.addValiables("y: "+to_string(dir.y), &dir.y);
    // display.addValiables("z: "+to_string(dir.z), &dir.z);
    // display.addValiables("accel_avr: "+to_string(dir.accel_avr), &dir.accel_avr);
    // display.addValiables("move: "+to_string(push_move), &_push_move);
    // display.addValiables("dir : "+to_string(push_dir), &_push_dir);

    display.debug();
    display.draw();
    is_display_on = true;

    state = State::KickOff;
    // state = State::Follow;
    // state = State::Test;
    // state = State::Pushing;
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
  static int dir_type = 0;  // ゴールに向けるやつ ドリブルの判定でも使うためココに配置
  if(state == State::Follow){
    float move_dir = 0;
    bool face_flag = true;

    // 直進

    // PD
    if(abs(ball.dir)<h){
      move_dir = ball.dir * p_gain - d_gain*(ball.dir - ball.dir_prev);
      // move_dir = ball.dir * p_gain;
      h = 45;
      face_flag = false;
    }
    // 円周上
    else if(ball.distance < r){
      float theta = 90 + (r-ball.distance) * 90 / r;
      move_dir = ball.dir + (ball.dir>0?theta:-theta);
      h = 30;
      // face_flag = false;
    }
    //接線
    else{
      float theta = degrees(asin(r / ball.distance));
      move_dir = ball.dir + (ball.dir>0?theta:-theta);
      h = 30;
    }
    motor.moveDir(move_dir, 75);

    // 白線処理
    if(line.on) line_flag = 1;


    // ゴールに向ける
    static float  face = 0;
    static uint32_t t_begin = 0;

    if(face_flag){
      switch(dir_type){
        case 0:
          face = 0;

          // ゴールに近い場合のみ
          if(camera.atk.h > 24 && millis()-t_begin > 500){
          // if(camera.atk.h > 23){
            if(camera.atk.dir < -16){
            // if(camera.atk.x1 > 160){
              dir_type = 1;
              face = 20;
              t_begin = millis();
            }else if(camera.atk.dir > 16){
            // }else if(camera.atk.x1 < 245){
              dir_type = 2;
              face = -20;
              t_begin = millis();
            }
          }
          break;
        // ロボットを左に向ける
        case 1:
          face = 20;
          if(camera.atk.dir > 18 && millis()-t_begin > 0){
          // if(camera.atk.x2 < 170){
            dir_type = 0;
            t_begin = millis();
          }
          break;
        // ロボットを右に向ける
        case 2:
          face = -20;
          if(camera.atk.dir < -18 && millis()-t_begin > 0){
          // if(camera.atk.x1 > 170){
            dir_type = 0;
            t_begin = millis();
          }
          break;
      }
    }
    // motor.setDirAdd(dir.dir + face, dir.dir_prev, dir.p_gain, dir.d_gain);
    motor.setDirAdd(dir.dir, dir.dir_prev, dir.p_gain, dir.d_gain);


    // ボールを保持 -> ゴールに向かう
    if(ball.is_hold){
      // Test
      state = State::Dribble;
      state_elapsed = 0;
    }

    // 押し込み
    if(state_elapsed > 4000 && abs(dir.dir) < 15 && camera.atk.is_visible && camera.atk.h > 50 && abs(ball.dir) < 25 && abs(line.dir) < 5){
      line_flag = 0;
      state = State::Oshikomi;
    }

    if(!ball.is_exist) state = State::NoBall;

  }



  // ToDo: ゴールに向かう
  static uint32_t dribble_end = millis(); // ドリブルの終了時間(差分を計算して白線避けに使用する)
  if(state == State::Dribble){
    // ボールを持ち始めたときのゴールまでの距離で方式を決定
    static bool is_decided = false;
    static int  type = 0;

    if(!is_decided){
      // ゴールに近すぎる場合、平行移動ですら躱せないためねじる
      if((camera.atk.h > 38 && !camera.is_center) || (abs(camera.chance_dir) > 20 && camera.atk.h > 36)){
      // if(camera.atk.h > 36){
        type = 0;
      }
      // ゴールに近い場合、姿勢制御が間に合わないため平行移動
      else if(camera.atk.h > 34){
        type = 1;
      }
      // 間に合う場合、ゴールに向けて姿勢制御しながら攻める
      else{
        type = 2;
      }

      is_decided = true;
    }


    // ねじる
    if(type == 0){
      motor.moveDirFast(0, 100);
      // motor.moveDir(0, 100);
      motor.setDirAdd(camera.chance_dir, camera.chance_dir_prev, -7.0f , 0);
    }


    // 平行移動
    else if(type == 1){
      motor.moveDirFast(camera.chance_dir, 100);
      motor.setDirAdd(dir.dir, dir.dir_prev, dir.p_gain, dir.d_gain);

      if(abs(camera.chance_dir) < 2 && state_elapsed > 200) sub.kick();
      if(camera.atk.h > 40) sub.kick();
    }


    // 空いてる方に向ける
    else if(type == 2){
      // motor.moveDirFast(camera.chance_dir, 100);
      motor.moveDirFast(-dir.dir, 100);

      // 一気にゴールに向けるとボールを離してしまうため、出力をコントロールする
      float p_power = state_elapsed / 80;
      if(p_power > 4) p_power = 4;
      motor.setDirAdd(camera.chance_dir, camera.chance_dir_prev, -dir.p_gain * p_power, dir.d_gain * 0);

      if(abs(camera.chance_dir) < 2 && state_elapsed > 200) sub.kick();
      if(camera.atk.h > 36) sub.kick();
    }
    else{
      type = 0;
    }


    // 白線処理
    if(line.on) line_flag = 2;


    // ボールを保持していない -> 回り込みなおす
    if(!ball.is_hold) {
      is_decided = false;
      state = State::Follow;
    }

    // ボールなし -> ボールなし
    if(!ball.is_exist) {
      is_decided = false;
      state = State::NoBall;
    }

    if(state_elapsed > 2500 || dir.accel_avr < 0.5){
      state = State::Pushing;
    }

    dribble_end = millis();

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
      if(dir.dir < 0){
        motor.moveDir(push_move, 100);
        motor.setDirAdd(dir.dir+push_dir, dir.dir_prev, dir.p_gain, dir.d_gain);
      }else{
        motor.moveDir(push_move, 100);
        motor.setDirAdd(dir.dir-push_dir, dir.dir_prev, dir.p_gain, dir.d_gain);
      }

    }else{
      motor.moveDir(0, 50);
      if(dir.dir < 0){
        motor.setDirAdd(dir.dir+90, dir.dir_prev, dir.p_gain*4, dir.d_gain);
      }else{
        motor.setDirAdd(dir.dir-90, dir.dir_prev, dir.p_gain*4, dir.d_gain);
      }
    }

    if(line.on) line_flag = 2;



    // 4秒経過 -> ドリブルに戻る
    if(state_elapsed > 3000 || abs(dir.dir) > 50){
      state = State::Dribble;
    }

    // ボールを奪えた -> シュート
    // if(!ball.is_hold){
    //   state = State::Follow;
    // }
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

    ui.buzzer(440.0f);
    motor.moveDir(0, 0);
    // motor.setDir(dir.dir, dir.dir_prev, dir.p_gain, dir.d_gain);


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



  // ok: 押し込み
  else if(state == State::Oshikomi){
    static bool go_flag = true;
    static int  count = 0;
    static uint32_t timer = millis();

    line_flag = 0;

    // 飛び出さないよう、0.5秒間止める
    if(state_elapsed < 500){
      motor.moveDir(0,0);
    }

    // 進む
    else if(go_flag){
      float move_dir = ball.dir;
      if(ball.dir > 10){
        move_dir = 10;
      }else if(ball.dir < -10){
        move_dir = -10;
      }
      motor.moveDir(move_dir, 50);
      motor.setDirAdd(dir.dir, dir.dir_prev, dir.p_gain*2, dir.d_gain);

      if(ball.is_hold) sub.kick();

      // ギリギリなら戻る
      if(abs(line.dir) > 90 && line.distance > 1.4 || millis() - timer > 1000){
        go_flag = false;
        timer = millis();
      }
    }

    // 下がる
    else{
      motor.moveDir(180, 60);
      motor.setDirAdd(dir.dir, dir.dir_prev, dir.p_gain*2, dir.d_gain);

      if((!line.on && abs(line.dir) < 90) || millis() - timer > 1000){
        go_flag = true;
        count++;
        timer = millis();
      }
    }


    if(count >= 3 || (!line.on && abs(line.dir) < 20 && abs(ball.dir) > 45)){
      count = 0;
      state = State::Follow;
    }

    if(!ball.is_exist){
      state = State::NoBall;
    }

  }


  else if(state == State::Ritsumori){
    if(!camera.atk.is_visible && !camera.def.is_visible){
      motor.moveDir(0, 90);
      motor.setDirAdd(dir.dir, dir.dir_prev, dir.p_gain, dir.d_gain);
    }else if(camera.def.h >= 20){
      if(ball.dir < 0){
        motor.moveDir(-45, 90);
        motor.setDirAdd(dir.dir, dir.dir_prev, dir.p_gain, dir.d_gain);
      }else{
        motor.moveDir(45, 90);
        motor.setDirAdd(dir.dir, dir.dir_prev, dir.p_gain, dir.d_gain);
      }
    }else{
      if(ball.dir < 0){
        motor.moveDir(-90, 90);
        motor.setDirAdd(dir.dir, dir.dir_prev, dir.p_gain, dir.d_gain);
      }else{
        motor.moveDir(90, 90);
        motor.setDirAdd(dir.dir, dir.dir_prev, dir.p_gain, dir.d_gain);
      }
    }

    if(line.on) line_flag = 1;


    if(state_elapsed > 4000 || ball.distance < 12500){
      state = State::Follow;
    }
  }


  // テスト
  else if(state == State::Test){

  }
  else{
    state = State::Follow;
  }



  // 白線処理
  static uint32_t timer = 0;
  static float    back_dir = 0;

  // 速い場合に0.5秒間戻る
  if(millis()-timer < 500){
    motor.moveDirFast(back_dir, 100);
  }
  // 普通
  else{
    switch(line_flag){
    case 1:{

      // ドリブル直後の場合
      if(millis()-dribble_end < 400){
        back_dir = line.dir + 180.0f;
        timer = millis();
      }


      // 反対側のポケットにアクセス
      // 正面を向いている場合
      if(!camera.atk.is_visible && abs(dir.dir) < 15 && abs(line.dir) < 10 && ball.distance > 14000){
        back_dir = 180;
        timer = millis();
      }
      // ゴールの方を向いている場合
      if(abs(line.dir) < 90 && ball.distance > 13000){
        if( (dir_type == 1 && -135 < ball.dir && ball.dir < 45)  ||  (dir_type == 2 && -45 < ball.dir && ball.dir < 135) ){
          back_dir = 180 - dir.dir;
          timer = millis();
        }
      }
      // 自陣コート側
      if(/*!camera.atk.is_visible && */abs(dir.dir) < 15 && abs(line.dir) > 125 && ball.distance > 14000){
        // back_dir = 0;
        // timer = millis();
        state = State::Ritsumori;
      }


      float avoid_dir = 0;

      // 後ろ
      if(line.dir > 157.5 || line.dir < -157.5){
        // if(abs(ball.dir) < 5) avoid_dir = 0;
        /*else*/ if(ball.dir < 0) avoid_dir = -45;
        else                  avoid_dir = 45;
      }
      // 左後ろ
      else if(line.dir < -112.5){
        // if(-135 < ball.dir && ball.dir < 45)  avoid_dir = 0;
        // else                                  avoid_dir = 90;
        if(-90 < ball.dir && ball.dir < 0)      avoid_dir = 0;
        else if(0 < ball.dir && ball.dir < 90)  avoid_dir = 45;
        else                                    avoid_dir = 135;
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
        // if(abs(ball.dir) < 5) avoid_dir = 180;
        if(ball.dir < 0) avoid_dir = 135;
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
        // if(-45 < ball.dir && ball.dir < 135)  avoid_dir = 0;
        // else                                  avoid_dir = -90;
        if(-90 < ball.dir && ball.dir < 0)      avoid_dir = -45;
        else if(0 < ball.dir && ball.dir < 90)  avoid_dir = 0;
        else                                    avoid_dir = -135;
      }

      motor.moveDir(avoid_dir, 90);  // Edit
      break;

    }
    case 2:
      back_dir = line.dir + 180.0f;
      timer = millis();
      break;
    }

  }


  motor.avr();
  motor.write();
}