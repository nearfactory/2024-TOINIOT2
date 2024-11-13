void setup() {
  Serial.begin(9600);
  Serial.println("TOINIOT2 Line Test Teensy 2024-09-04");

  // pinMode(0,INPUT_PULLDOWN);
  // pinMode(1,INPUT_PULLDOWN);

  // pinMode(0,OUTPUT);
  // pinMode(1,OUTPUT);

  Serial1.begin(9600);
}

void loop() {
  // if(Serial1.available()){
  // }
  while(Serial1.available()){
    Serial.print((char)Serial1.read());
    digitalWrite(LED_BUILTIN,HIGH);
  }
  digitalWrite(LED_BUILTIN, LOW);

  // digitalWrite(LED_BUILTIN, HIGH);
  // digitalWrite(0,HIGH);
  // digitalWrite(1,HIGH);
  // delay(500);
  // digitalWrite(0,LOW);
  // digitalWrite(1,LOW);
  // digitalWrite(LED_BUILTIN, LOW);
  // delay(500);

  // Serial.print(digitalRead(0));
  // Serial.println(digitalRead(1));

  delay(20);
}