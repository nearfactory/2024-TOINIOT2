/*

Connect LED to GPIO2.

DO NOT connect LED without resistor!!!
100ohms or 330 ohms are recommended.

*/

const uint8_t LED_PIN = 2;

void setup() {
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
  delay(500);
}
