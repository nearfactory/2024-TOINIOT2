#define ANALOG_PIN    A0
#define DIGITAL_PIN   3
#define THRESHOLD_PIN 2

void setup() {
  Serial.begin(9600);
  pinMode(ANALOG_PIN, INPUT);
  pinMode(DIGITAL_PIN, INPUT_PULLUP);
  pinMode(THRESHOLD_PIN, OUTPUT);
}

void loop() {
  Serial.print("analog:");
  Serial.print(analogRead(ANALOG_PIN));
  Serial.print(" digital:");
  Serial.print(digitalRead(DIGITAL_PIN));
  Serial.println();

  analogWrite(THRESHOLD_PIN, 128);

  delay(25);
}
