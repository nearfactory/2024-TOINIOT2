void setup() {
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  Serial.begin(9600);
}

void loop() {
  Serial.printf("ball02k:%04d\tball01k:%04d\n", analogRead(A0), analogRead(A1));
  delay(50);
}