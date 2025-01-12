void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
}

void loop() {

  Serial1.println("atmega");
  while(Serial1.available()){
    Serial.print((char)Serial1.read());
  }
  // Serial.println();

  delay(20);
}
