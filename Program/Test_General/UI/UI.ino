constexpr int UIpin[5] = {37, 36, 30, 31, 32};

void setup() {
  for(int i=0; i<5; i++){
    pinMode(UIpin[i], INPUT);
  }

  Serial.begin(115200);
  Serial.println("UI Test Program 2025-01-27");
}

void loop() {
  for(int i=0; i<5; i++){
    Serial.print(digitalRead(UIpin[i]));
    Serial.print("\t");
  }
  Serial.println();
  delay(50);
}
