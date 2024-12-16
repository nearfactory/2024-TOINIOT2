// #include "Motor.hpp"

// Motor motor;

void setup(){
  // motor.begin();
}

void loop(){
  // motor.set(60.0f, 60.0f, 60.0f, 60.0f);
  // motor.avr();
  // motor.write();

  // for(int i=2;i<=9;i++){
  //   digitalWrite(i,1);
  // }
  
    // {29, 28},
    // {9, 6},
    // {5, 4},
    // {3, 2}
    analogWrite(29,128);
    analogWrite(28,128);
    analogWrite(9,128);
    analogWrite(6,128);
    analogWrite(5,128);
    analogWrite(4,128);
    analogWrite(3,128);
    analogWrite(2,128);
  // motor.set(60.0f,60.0f,60.0f,60.0f);
  // motor.avr();
  // motor.write();
  delay(100); 
}