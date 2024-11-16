// #include "C:\GitHub\2024-TOINIOT2\Program\@include\Line.hpp"
// #include "C:\GitHub\2024-TOINIOT2\Program\@include\Motor.hpp"
// #include "C:\GitHub\2024-TOINIOT2\Program\@include\UI.hpp"

#include "Dir.hpp"
#include "Line.hpp"
#include "Motor.hpp"
#include "UI.hpp"

Line line;
Motor motor;
Dir dir;


void setup() {
  Serial.begin(115200);
  Serial.println("TOINIOT2 Line Test Teensy 2024-09-04");

  line.begin(115200);
  motor.begin();
  dir.begin();
  // dir.calibration();

  delay(1000);
  dir.setDefault();
}

void loop() {
  
  // 白線避け
  // line.read();
  // if(digitalRead(TOGGLE_PIN)){
  //   motor.set(0,0,0,0);
  // }else if(line.num){
  //   motor.moveDir(line.dir+180, 60);
  // }else{
  //   motor.set(0,0,0,0);
  // }

  if(digitalRead(TOGGLE_PIN)){
    motor.set(0,0,0,0);
  }else{
    dir.read();
    motor.moveDir(-dir.dir, 50);
    motor.add(10,10,10,10);
  }

  motor.avr();
  motor.write();

  // dir.read();
  // Serial.println(dir.dir);

  delay(50);
}