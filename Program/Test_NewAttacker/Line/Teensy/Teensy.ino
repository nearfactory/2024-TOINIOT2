// #include "C:\GitHub\2024-TOINIOT2\Program\@include\Line.hpp"
// #include "C:\GitHub\2024-TOINIOT2\Program\@include\Motor.hpp"
// #include "C:\GitHub\2024-TOINIOT2\Program\@include\UI.hpp"

#include "Line.hpp"
#include "Motor.hpp"
#include "UI.hpp"

Line line;
Motor motor;


void setup() {
  Serial.begin(115200);
  Serial.println("TOINIOT2 Line Test Teensy 2024-09-04");

  line.begin(115200);
  motor.begin();
}

void loop() {
  
  // 白線避け
  line.read();
  if(line.num){
    motor.moveDir(line.dir+180, 60);
  }

  motor.avr();
  motor.write();



  // if(line_num == 0) {
  //   motor[0] = 0;
  //   motor[1] = 0;
  //   motor[2] = 0;
  //   motor[3] = 0;
  // }else if(!digitalRead(SW_TAC_PIN[0])){
  //   moveDir(degrees(line_dir)+180, 60, 100);
  // }else{
  //   motor[0] = 0;
  //   motor[1] = 0;
  //   motor[2] = 0;
  //   motor[3] = 0;
  // }

  // motorP();
  // motorRaw();

  delay(50);
}