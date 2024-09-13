void setup() {
  pinMode(A0, INPUT);
  pinMode(2, OUTPUT);
  Serial.begin(115200);
}

float t=0;

void loop() {
  t += 1;
  // Serial.print(4096);
  // Serial.print("\t");
  // Serial.print(0);
  // Serial.print("\t");
  // float var = analogRead(A0)*511;
  // var = var / 4096;
  // int var2 = (int)var;
  // Serial.print(var2);
  // Serial.print("\t");
  // Serial.println(analogRead(A0));
  Serial.println((int)abs(sin(t*3.149265/180)*255));
  analogWrite(2, (int)abs(sin(t*3.149265/180)*255));
  delay(10);
}