void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN,OUTPUT);
  for(int i=2;i<=9;i++){
    pinMode(i,OUTPUT);
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  
  digitalWrite(LED_BUILTIN,HIGH);
  for(int i=2;i<=9;i++){
    analogWrite(i,128);
  }

  delay(1000);
  
  // digitalWrite(LED_BUILTIN,LOW);
  // for(int i=2;i<=9;i++){
  //   digitalWrite(i,0);
  // }

  // delay(1000);
}
