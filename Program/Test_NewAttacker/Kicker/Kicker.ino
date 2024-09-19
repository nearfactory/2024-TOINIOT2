constexpr uint8_t KICKER_PIN = 32;

void setup() {
  Serial.begin(9600);
  pinMode(KICKER_PIN, OUTPUT);
  Serial.println("begin");
}

void loop() {
  digitalWrite(KICKER_PIN, LOW);
  delay(5000);
  digitalWrite(KICKER_PIN, HIGH);
  delay(40);
  Serial.println("kick");
}
