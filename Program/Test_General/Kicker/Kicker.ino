void setup() {
  pinMode(3, OUTPUT);
  digitalWrite(3, 0);

  Serial.begin(115200);
  Serial.println("Kicker Test Program 2025-01-27");
}

void loop() {
  digitalWrite(3, 1);
  delay(50);
  digitalWrite(3, 0);
  delay(5000);
}
