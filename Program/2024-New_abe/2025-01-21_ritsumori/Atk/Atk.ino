// new atacker kansai block program
// Teensy4.1
// 2024-01-05

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
float h = 45;         // ヒステリシス
// float r = 11800.0;    // 回り込みの半径
float r = 8200.0;    // 回り込みの半径
float p_gain = 1.5;
float d_gain = 3.0;  // Test
// float d_gain = 1.0;

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

  // ディスプレイ
  if(ui.is_toggle){
    Serial.println("display");
    camera.lock = false;
    if(ui.buttonUp(0)) display.next();

    display.addValiables("p_gain :"+to_string(p_gain), &p_gain);
    display.addValiables("d_gain :"+to_string(d_gain), &d_gain);
    display.addValiables("offset: "+to_string(offset), &offset);
    // display.addValiables("speed_normal :"+to_string(speed_normal), &speed_normal);

    display.debug();
    display.draw();
    is_display_on = true;

    // state = State::KickOff;
    state = State::Follow;
    // state = State::Test;
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
      h = 10;
      face_flag = false;
    }
    // 円周上
    else if(ball.distance < r){
      float theta = 90 + (r-ball.distance) * 90 / r;
      move_dir = ball.dir + (ball.dir>0?theta:-theta);
      h = 5;
      // face_flag = false;
    }
    //接線
    else{
      float theta = degrees(asin(r / ball.distance));
      move_dir = ball.dir + (ball.dir>0?theta:-theta);
      h = 5;
    }
    motor.moveDir(move_dir, 90); 

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
          if(camera.atk.h > 25 && millis()-t_begin > 500){
          // if(camera.atk.h > 23){
            if(camera.atk.dir < -18){
            // if(camera.atk.x1 > 160){
              dir_type = 1;
              face = 20;
              t_begin = millis();
            }else if(camera.atk.dir > 20){
            // }else if(camera.atk.x2 < 160){
              dir_type = 2;
              face = -20;
              t_begin = millis();
            }
          }
          break;
        // ロボットを左に向ける
        case 1:
          face = 20;
          if(camera.atk.dir > 14 && millis()-t_begin > 0){
          // if(camera.atk.x2 < 170){
            dir_type = 0;
            t_begin = millis();
          }
          break;
        // ロボットを右に向ける
        case 2:
          face = -20;
          if(camera.atk.dir < -14 && millis()-t_begin > 0){
          // if(camera.atk.x1 > 170){
            dir_type = 0;
            t_begin = millis();
          }
          break;
      }
      // if(!camera.atk.is_visible){
      //   dir_type = 0;
      // }
    }
    motor.setDirAdd(dir.dir + face, dir.dir_prev, dir.p_gain, dir.d_gain);

   
    // ボールを保持 -> ゴールに向かう
    if(ball.is_hold){
      // Test
      state = State::Dribble;
    }

    // 押し込み
    if(state_elapsed > 4000 && abs(dir.dir) < 15 && camera.atk.h > 50 && abs(ball.dir) < 25 && abs(line.dir) < 5){
      line_flag = 0;
      state = State::Oshikomi;
    }

    if(!ball.is_exist) state = State::NoBall;

  }


  /*
  // ToDo: ゴールに向かう
  else if(state == State::Dribble){
    // 攻める角度の決定

    static bool is_decided = false;
    static int  type = 0;

    camera.lock = true;

    // 方式の決定
    if(!is_decided){
      // キッカーok かつ 姿勢制御が間に合う
      if(sub.ready && camera.is_center){
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

        motor.setDirAdd(camera.chance_dir, camera.chance_dir, dir.p_gain / 1.5, 0);
        

        // キックに移行
        if(camera.atk.h > 22 && camera.is_center){
          kick_begin = true;
          kick_timer = millis();
        }

        if(!ball.is_hold){
          state = State::Follow;
        }
      }


      // キック
      else{
        // 0.1秒進んでキック
        if(millis()-kick_timer < 50){
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
        if(camera.atk.h > 22){
          nejiri_begin = true;
          nejiri_timer = millis();
        }

        if(!ball.is_hold){
          state = State::Follow;
        }
      }

      // ねじる
      else{
        // 0.4秒ねじったら回り込みに戻る
        if(millis()-nejiri_timer < 150){
          if(line.on) line_flag = 2;
          motor.moveDirFast(0, 100);
          motor.setDirAdd(camera.atk.dir, camera.atk.dir_prev, -8.0f, 0.0f);
        }else{
          nejiri_begin = false;
          state = State::Follow;
        }

      }

    }


    // ボールを保持していない -> 回り込みなおす
    // if(!ball.is_hold) state = State::Follow;

    // ロボットが動かない(≒このステートで5秒経過 -> 押し合い
    // if(state_elapsed > 5000){
    //   state = State::Pushing;
    // }

    // ボールなし -> ボールなし
    if(!ball.is_exist) state = State::NoBall;


    // キーパーが目の前にいる -> キーパー避け
  }
  */
  else if(state == State::Dribble){
    // ボールを持ち始めたときのゴールまでの距離で方式を決定
    static bool is_decided = false;
    static int  type = 0;

    if(!is_decided){
      // ゴールに近すぎる場合、平行移動ですら躱せないためねじる
      if(camera.atk.h > 36 && !camera.is_center){
      // if(camera.atk.h > 36){
        type = 0;
      }
      // ゴールに近い場合、姿勢制御が間に合わないため平行移動
      else if(camera.atk.h > 32){
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
      // motor.moveDirFast(0, 100);
      motor.moveDir(0, 100);
      motor.setDirAdd(camera.chance_dir, camera.chance_dir_prev, -4.0f , 0);
    }


    // 平行移動
    else if(type == 1){
      // motor.moveDirFast(camera.chance_dir, 100);
      motor.moveDir(camera.chance_dir, 100);
      motor.setDirAdd(dir.dir, dir.dir_prev, dir.p_gain, dir.d_gain);

      // if(camera.is_center) sub.kick();
    }


    // 空いてる方に向ける
    else if(type == 2){
      // motor.moveDirFast(0,100);
      motor.moveDir(0,100);

      // 一気にゴールに向けるとボールを離してしまうため、出力をコントロールする
      float p_power = state_elapsed / 80;
      if(p_power > 4) p_power = 4;
      motor.setDirAdd(camera.chance_dir, camera.chance_dir_prev, -dir.p_gain * p_power, dir.d_gain * 0);

      // if(camera.is_center) sub.kick();
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
  /*
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
  */



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
      // motor.moveDir(move_dir, 50);
      motor.moveDir(move_dir, 100);
      motor.setDirAdd(dir.dir, dir.dir_prev, dir.p_gain*2, dir.d_gain);

      if(ball.is_hold) sub.kick();

      // ギリギリなら戻る
      if(abs(line.dir) > 90 && line.distance > 1.4){
        go_flag = false;
      }
    }

    // 下がる
    else{
      // motor.moveDir(180, 60);
      motor.moveDir(180, 100);
      motor.setDirAdd(dir.dir, dir.dir_prev, dir.p_gain*2, dir.d_gain);

      if(!line.on && abs(line.dir) < 90){
        go_flag = true;
        count++;
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


  // テスト
  else if(state == State::Test){
    // camera.lock = true;
    Serial.println("test");
    // Serial.printf("atk_num:%d", camera.atk.num);
    motor.setDir(camera.chance_dir, camera.chance_dir_prev, -dir.p_gain*3, dir.d_gain*0);
    // motor.moveDir(0, 0);
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
  }else{
    switch(line_flag){
    case 1:{

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

      motor.moveDirFast(avoid_dir, 100);
      break;
      
      // // 後ろ側でラインに触れる かつ ボールが前
      // if(abs(line.dir)>135){
      //   float avoid_dir = 0;
      //   if(abs(ball.dir) < 10) avoid_dir = 0;
      //   else if(ball.dir > 0)  avoid_dir = 45;
      //   else                   avoid_dir = -45;

      //   motor.moveDirFast(avoid_dir, 100);
      // }
      // // 左側
      // else if(line.dir < -45){
      //   if(abs(ball.dir) < 90){
      //     motor.moveDirFast(45, 100);
      //   }else{
      //     motor.moveDirFast(135, 100);
      //   }
      // }
      // // 右側
      // else if(45 < line.dir){
      //   if(abs(ball.dir) < 90){
      //     motor.moveDirFast(-45, 100);
      //   }else{
      //     motor.moveDirFast(-135, 100);
      //   }
      // }
      // // 前側でラインに触れる
      // else{
      //   float avoid_dir = 0;
      //   if(abs(ball.dir)<22.5)  avoid_dir = 180;
      //   else if(ball.dir > 0)   avoid_dir = 135;
      //   else                    avoid_dir = -135;
      //   motor.moveDirFast(avoid_dir, 100);
      // }

      // break;
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