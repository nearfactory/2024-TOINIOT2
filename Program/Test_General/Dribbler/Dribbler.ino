#define EN 32
#define PH 27

void setup() {
  pinMode(EN, OUTPUT);
  pinMode(PH, OUTPUT);

}

void loop() {
  digitalWrite(PH,0);
  analogWrite(EN,128);

  delay(50);
}
