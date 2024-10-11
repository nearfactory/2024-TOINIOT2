void setup() {
  pinMode(A10,INPUT);
  pinMode(A11,INPUT);
}

void loop() {
  Serial.printf("ball02k:%04d\tball01k:%04d\n", analogRead(A10), analogRead(A11));
  delay(50);
}