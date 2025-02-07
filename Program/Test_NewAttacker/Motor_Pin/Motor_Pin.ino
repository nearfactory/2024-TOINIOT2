void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN,OUTPUT);
  for(int i=2;i<=9;i++){
    pinMode(i,OUTPUT);
    analogWriteFrequency(i, 40000);
  }

}

void loop() {  
  Serial.println("motor pin 2");
  digitalWrite(LED_BUILTIN,HIGH);
  for(int i=2;i<=9;i++){
    // analogWrite(i,128);
    // digitalWrite(i,1);
  }

  analogWrite(2,128);
  analogWrite(3,128);
  analogWrite(4,128);
  analogWrite(5,128);
  analogWrite(6,128);
  analogWrite(7,128);
  analogWrite(8,128);
  analogWrite(9,128);

  delay(100);
  digitalWrite(LED_BUILTIN,LOW);
  
  // digitalWrite(LED_BUILTIN,LOW);
  // for(int i=2;i<=9;i++){
  //   digitalWrite(i,0);
  // }

}
