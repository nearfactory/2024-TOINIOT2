// #include "Motor.hpp"

// Motor motor;

void setup(){
  // motor.begin();
  for(int i=2;i<=9;i++){
    analogWriteFrequency(i, 40000);
    pinMode(i,OUTPUT);
  }
}

void loop(){
  // motor.set(60.0f, 60.0f, 60.0f, 60.0f);
  // motor.avr();
  // motor.write();

  for(int i=2;i<=9;i++){
    digitalWrite(i,1);
  }
  delay(100); 
}