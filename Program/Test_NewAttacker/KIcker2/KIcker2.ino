uint8_t pin = 32;

void setup() {
  pinMode(pin, OUTPUT);
}

void loop() {
  digitalWrite(pin, 1);
  delay(5000);
  digitalWrite(pin, 0);
  delay(5000);
}
