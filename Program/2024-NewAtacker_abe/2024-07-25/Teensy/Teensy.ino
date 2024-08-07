// new atacker main program
// Teensy4.1
// 2024-07-30

#include <Wire.h>

#include "Ball.hpp"
#include "Communication.hpp"
#include "Dir.hpp"
#include "Kicker.hpp"
#include "Motor.hpp"
#include "UI.hpp"

using namespace std;

void setup() {
  communicationSetup();
  ballSetup();
  dirSetup();
  kickerSetup();
  motorSetup();
  UISetup();

  // calibration
  /*
  */
  DISPLAY_MODE = DISPLAY_MODE::DIR;
  uint8_t system = 0, gyro = 0, accel = 0, mag = 0;
  while(system!=3 || gyro!=3 || mag!=3){
    display.clearDisplay();
    printd(8,8,"calibration...");
    Serial.println("calibration...");

    // dirCalibration(&system, &gyro, &accel, &mag);
    bno.getCalibration(&system, &gyro, &accel, &mag);
    printd(32,24,"system:"+to_string(system));
    printd(32,32,"gyro  :"+to_string(gyro));
    printd(32,40,"accel :"+to_string(accel));
    printd(32,48,"mag   :"+to_string(mag));

    display.display();
    delay(10);
  }

  display.clearDisplay();
  printd(120,56,"start",TEXT_ALIGN_X::RIGHT, TEXT_ALIGN_Y::BOTTOM);
  display.display();
  while(!buttonUp(4)){
    buttonUpdate();
  }


  // set default dir
  dirUpdate();
  default_dir = dir;
  dir_default_display = -dir;
}

void loop() {
  static auto begin_ms = millis();
  digitalWrite(LED_BUILTIN, HIGH);
  display.clearDisplay();

  ballUpdate(BALL::DIR);
  buttonUpdate();
  clearVariables();
  // subUpdate();

  dirUpdate();

  // analogWrite(0,0);

  // motor_min
  // motor_powerz

  static short motor_dir = 0;
  motor_dir += button[0];
  motor_dir -= button[1];

  // 回り込み
  // short motor_power = (ball_distance-6500) *0.8 / 98;
  // 線形にずらした角度に進める
  // Serial.printf("dir:%d\n", motor_dir);
  
  /*
  double default_difference = (BALL_DISTANCE_MAX-ball_distance)/BALL_DISTANCE_RANGE*85+5;
  double difference = 0.0;
  if(abs(ball_dir)>45){
    difference = ball_dir<0?-default_difference:default_difference;
  }else{
    difference = default_difference*ball_dir/45;
  }
  move_dir = ball_dir + difference;
  moveDir(move_dir, 70, true);

  addVariables("Move_dir",move_dir);
  addVariables("diffe",difference);
  */
  short difference=0;
  if(abs(ball_dir)>15){
    difference = ball_dir<0?-70:70;
  }else{
    difference = 20*ball_dir/15;
    // difference = ball_dir<0?-difference:difference;
  }
  moveDir(ball_dir+difference,90,false);


  // 90-5
  if(ball_holding) {
    moveDir(0.0, 90, true);
    if(millis()-kicked_ms>5000 && millis()-ball_hold_begin > 300){
      kick();
    }
  }
  if(!ball_exist) motorRaw(0,0,0,0);
  setDir(dir,default_dir,60,40);
  // motorRaw(40,40,40,40);
  
  motorRaw();
  // motorRapidP(motor[0],motor[1],motor[2],motor[3]);
  // motorRaw();
  // for(int i=0;i<kMOTOR_NUM;i++) motor_now[i] = motor[i];


  // UI (display)
  addVariables("process", millis()-begin_ms);
  addVariables("ball_hold",millis()-ball_hold_begin);
  begin_ms = millis();
  if(buttonUp(4)) DISPLAY_MODE = (DISPLAY_MODE+1)%DISPLAY_MODE_NUM;
  debugDisplay(DISPLAY_MODE);
  display.display();

  // printd(8, 8, "process:"+std::to_string(millis()-begin_ms)+"(ms)");
  // delay(50);
}