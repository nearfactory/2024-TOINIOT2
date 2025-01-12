void setup(){
  Serial.begin(115200);
  Serial1.begin(115200);

  delay(500);
  Serial.println("2024-12-24 TOINIOT2 Line Connection Test");
}

void loop(){
  Serial1.println("Teensy");
  
  while(Serial1.available()){
    Serial.print((char)Serial1.read());
  }
  // Serial.println();

  delay(30);
}
