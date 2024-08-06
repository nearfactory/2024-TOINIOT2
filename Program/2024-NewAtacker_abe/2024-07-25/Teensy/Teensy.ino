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
  // kickerSetup();
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
  // subUpdate();

  dirUpdate();

  // analogWrite(0,0);

  // motor_min
  // motor_powerz

  static short motor_dir = 0;
  motor_dir += button[0];
  motor_dir -= button[1];

  short motor_power = (ball_distance-6500) *0.8 / 98;
  // 線形にずらした角度に進める
  // Serial.printf("dir:%d\n", motor_dir);
  double difference = 0.0;
  if(abs(ball_dir)>30){
    difference = ball_dir<0?-60:60;
  }else{
    difference = 60*ball_dir/45;
  }
  double move_dir = ball_dir + difference;
  moveDir(move_dir, 80, true);
  // motorRaw();

  // setDir();
  // motorRaw(-100,-100,100,100);
  
  /*
  if(buttonUp(3)){
    for(int i=0;i<100;i++){
      for(int j=0;j<MOTOR_NUM;j++){
        if(motor[j]>0) motor[j]--;
        // if(motor[j]<0) motor[j]++;
        motor[j]<0?0:motor[j];
        // motor[j]*=0.1;
      }
      motorRaw();
      delay(5);
    }
    while(!buttonUp(3)){
      buttonUpdate();
    }
  }
  */

  bz=0.0f;
  if(ball_holding) {
    moveDir(0.0, 90, false);
    bz=440.0f;
  }
  bzUpdate();
  if(!ball_exist) motorRaw(0,0,0,0);
  setDir(dir,default_dir,60,40);
  // motorRaw(40,40,40,40);
  motorRaw();
  

  Serial.printf("ball_min:-270\nball_max:90\n");
  Serial.printf("ball:%lf\n", ball_dir);
  Serial.printf("ball_distance:%d\n", ball_distance);
  Serial.printf("ball_holding:%d\n", ball_holding);
  Serial.printf("ball_exist:%d\n", ball_exist);

  // UI (display)
  clearVariables();
  addVariables("move_dir",move_dir);
  addVariables("process", millis()-begin_ms);
  begin_ms = millis();
  if(buttonUp(4)) DISPLAY_MODE = (DISPLAY_MODE+1)%DISPLAY_MODE_NUM;
  debugDisplay(DISPLAY_MODE);
  display.display();

  // printd(8, 8, "process:"+std::to_string(millis()-begin_ms)+"(ms)");
  // delay(50);
}