// IN- > IN+ → 0
// IN- < IN+ → 1

void setup() {
  pinMode(A0, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("こんぱれーたー てすと 2024-07-29");
}

void loop() {
  static int hensuu = 128;
  if (digitalRead(A1)) hensuu += hensuu < 255 ? 1 : 0;
  if (digitalRead(A2)) hensuu -= hensuu > 0 ? 1 : 0;

  Serial.print("threshold:");
  Serial.println(hensuu * 4);
  analogWrite(4, hensuu);
  Serial.println(digitalRead(A0));
  Serial.println(analogRead(A3));

  delay(50);
}
