void setup(){
  // uart
  Serial2.begin(9600);

  // test
  // pinMode(0,INPUT);
  // pinMode(1,OUTPUT);

  Serial.begin(9600);
}

void loop(){
  // uart
  while(Serial2.available()){
    char c = Serial2.read();
    //Serial.println(Serial1.read());
    Serial.print(c);
  }
  /*
  if(Serial.available()){
    Serial1.print(Serial.read());
  }
  */
  Serial2.println("Teensy4.1");

  // test
  // static bool val=false;
  // static int ms=0;
  // if(millis()-ms>250){
  //   val=!val;
  //   ms=millis();
  // }
  // Serial.println(digitalRead(0));
  // digitalWrite(1,val);
  
  delay(50);
}