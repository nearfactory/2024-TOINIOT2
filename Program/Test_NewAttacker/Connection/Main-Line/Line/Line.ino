void setup() {
  Serial.begin(115200);
}

void loop() {

  Serial.print("atmega:");
  while(Serial.available()){
    Serial.print((char)Serial.read());
  }
  Serial.println();

  delay(20);
}
