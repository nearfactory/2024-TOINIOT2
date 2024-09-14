#pragma once

// ボールセンサ個数
constexpr uint8_t BALL_NUM = 16;  
// 各センサピン番号
const uint8_t BALL_PIN[BALL_NUM] = { A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15 };

// ボール角度算出アルゴリズム列挙体
enum class BALL{
  DIR = 0,
  DIR2,
  DIR3
};

// ボール距離判断しきい値
constexpr short BALL_DISTANCE_MIN = 7000;
constexpr short BALL_DISTANCE_MAX = 15800;
constexpr short BALL_DISTANCE_RANGE = BALL_DISTANCE_MAX-BALL_DISTANCE_MIN;



// センサ値格納用配列（0で初期化）
short ball[BALL_NUM] = { 0 };
// 測定ボール角度
float ball_dir = 0;
// 直前の測定ボール角度
float prev_ball_dir = 0;
// ボール距離
short ball_distance = 0;
// ボール保持フラグ
bool  ball_holding = false;
// ボール存在フラグ
bool  ball_exist = true;

// 比較用最大値格納用変数
short   ball_big = 0;
// 最大値のセンサインデックス
uint8_t ball_big_id = 0;
// 比較用最小値格納用変数
short   ball_small = 1023;
// 最小値のセンサインデックス
uint8_t ball_small_id = 0;
// 直前のボール保持フラグ
bool prev_ball_holding = false;
// ボールを保持し始めた時刻
uint32_t ball_hold_begin = 0;



// セットアップ関数（.inoのsetup()内で呼び出し）
inline void ballSetup(){
  // pinMode変更
  for(auto p:BALL_PIN) pinMode(p, INPUT);

  Serial.println("ball setup");
  return;
}



// ボール角度測定関数（float型の角度を返り値として出す）
// すべてのセンサ値を活用して算出
inline float ballDirection(){
  // センサ値のx成分/y成分を初期化
  short ball_x = 0;
  short ball_y = 0;

  // 各センサ値をx成分/y成分に分解し2軸の成分に加算
  for(int i=0;i<BALL_NUM;i++){
    // センサ角度 (rad)
    double sensor_dir = PI*(i+4)/8+90;
    ball_x += ball[i] * cos(sensor_dir);
    ball_y += ball[i] * sin(sensor_dir);
  }
  
  // atan2()を用いて角度を算出(rad)し、degに変換
  // 注意： atan2()の返り値は -π から π (rad)
  float val = -atan2(ball_y, ball_x) * (180.0 / PI);
  // if(val<-180) val += 360.0;
  // if(val>180)  val -= 360.0;
  return val;
}



// ボール角度測定関数（float型の角度を返り値として出す）
// 最大のセンサ値を出すセンサの左右2つずつを活用して算出
inline float ballDirection2(){
  // センサの最大値と最大値を記録したセンサのインデックスを初期化
  short ball_big = 0;
  short ball_big_index = 0;

  //各センサ値の値を最大値を比較・更新し最大値とインデックスを算出
  for(int i=0;i<BALL_NUM;i++){
    if(ball_big < ball[i]){
      ball_big = ball[i];
      ball_big_index = i;
    }
  }
  
  // センサ値のx成分/y成分を初期化
  short ball_x = 0;
  short ball_y = 0;

  // 各センサ値をx成分/y成分に分解し2軸の成分に加算
  // i は -2 から 2 まで回す
  for(int i=-2;i<=2;i++){
    // j が 実際のセンサインデックス
    // BALL_NUMを加算している→割られる数が負になると剰余も負で出力されてしまうのを防ぐため
    int j = (ball_big_index + BALL_NUM + i) % BALL_NUM;

    // ★寺田編集済み
    double sensor_dir = PI*(j+4)/8+90;
    ball_x += ball[j] * cos(sensor_dir);
    ball_y += ball[j] * sin(sensor_dir);
  }

  // atan2()を用いて角度を算出(rad)し、degに変換
  // 注意： atan2()の返り値は -π から π (rad)
  return atan2(ball_y, ball_x) * (180.0 / PI);
}



// ボール角度測定関数（float型の角度を返り値として出す）
// 最大のセンサ値を出すセンサの左右3つずつを活用して算出
inline float ballDirection3(){
  // センサの最大値と最大値を記録したセンサのインデックスを初期化
  short ball_big = 0;
  short ball_big_index = 0;
  
  //各センサ値の値を最大値を比較・更新し最大値とインデックスを算出
  for(int i=0;i<BALL_NUM;i++){
    if(ball_big < ball[i]){
      ball_big = ball[i];
      ball_big_index = i;
    }
  }

  // センサ値のx成分/y成分を初期化
  short ball_x = 0;
  short ball_y = 0;

  // 各センサ値をx成分/y成分に分解し2軸の成分に加算
  // i は -2 から 2 まで回す
  for(int i=-3;i<=3;i++){
    // j が 実際のセンサインデックス
    // BALL_NUMを加算している→割られる数が負になると剰余も負で出力されてしまうのを防ぐため
    int j = (ball_big_index + BALL_NUM + i) % BALL_NUM;

    // ★寺田編集済み
    double sensor_dir = PI*(j+4)/8+90;
    ball_x += ball[j] * cos(sensor_dir);
    ball_y += ball[j] * sin(sensor_dir);
  }

  // atan2()を用いて角度を算出(rad)し、degに変換
  // 注意： atan2()の返り値は -π から π (rad)
  return atan2(ball_y, ball_x) * (180.0 / PI);
}



// ボールセンサまわりを更新する関数
// 引数には角度算出モードを指定（0:全センサ活用、1:最大センサ+左右2コ、2:最大センサ+左右3コ）
inline void ballUpdate(BALL mode){
  // センサ最大値/最大値/各インデックス/ボール距離を初期化
  ball_small = 1023;
  ball_small_id = 0;
  ball_big = 0;
  ball_big_id = 8;
  ball_distance = 0;

  // 各センサ値を比較して最大値/最小値/各インデックスを算出
  for(int i=0;i<BALL_NUM;i++){
    ball[i] = analogRead(BALL_PIN[i]);
    ball_distance += ball[i];
    if(ball_small>ball[i]){
      ball_small = ball[i];
      ball_small_id = i;
    }if(ball_big<ball[i]){
      ball_big = ball[i];
      ball_big_id = i;
    }
  }

  // 直前のボール保持フラグを更新
  prev_ball_holding = ball_holding;
  // ボール保持フラグを更新
  ball_holding = ball_distance < BALL_DISTANCE_MIN && abs(ball_dir)<30;
  // ボール存在フラグを更新
  ball_exist = ball_distance < BALL_DISTANCE_MAX;
  // ball_holding 5200-6800
  // exist < 16300

  // ボールの保持有無が切り替わる or 保持していない時
  if(!(ball_holding&&prev_ball_holding)){
    ball_hold_begin = millis();
  }

  // ボール角度を算出
  switch(mode){
    case BALL::DIR :
      ball_dir = ballDirection();
      break;
    case BALL::DIR2 :
      ball_dir = ballDirection2();
      break;
    case BALL::DIR3 :
      ball_dir = ballDirection3();
      break;
  }

  return;
}