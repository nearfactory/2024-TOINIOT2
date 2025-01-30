void setup() {
  pinMode(D3, OUTPUT);

  Serial.begin(115200);
  Serial.println("Kicker Test Program 2025-01-27");
}

void loop() {
  if(analogRead(A1) < 200){
    Serial.println("Holding");
    digitalWrite(D3, 1);
    delay(50);
    digitalWrite(D3, 0);
  }
  else{
    Serial.println("Waiting");
  }
  delay(1000);
}
