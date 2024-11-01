void setup(){
  // uart
  Serial7.begin(115200);

  // test
  // pinMode(0,INPUT);
  // pinMode(1,OUTPUT);

  Serial.begin(115200);
}

void loop(){
  // uart
  while(Serial7.available()){
    char c = Serial7.read();
    //Serial.println(Serial1.read());
    Serial.print(c);
  }
  /*
  if(Serial.available()){
    Serial1.print(Serial.read());
  }
  */
  Serial7.println("Teensy4.1");

  // test
  // static bool val=false;
  // static int ms=0;
  // if(millis()-ms>250){
  //   val=!val;
  //   ms=millis();
  // }
  // Serial.println(digitalRead(0));
  // digitalWrite(1,val);
  
  delay(100);
}