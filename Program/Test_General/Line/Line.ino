void setup() {
  Serial.begin(9600);
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
}

void loop() {
  Serial.print("min:0  max:1023  line1:");
  Serial.print(analogRead(A0));
  Serial.print("  line2:");
  Serial.println(analogRead(A1));
  delay(50);
}