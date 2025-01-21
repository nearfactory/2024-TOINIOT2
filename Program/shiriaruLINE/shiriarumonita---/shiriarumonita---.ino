int i;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(digitalRead(26));
  Serial.print("\t");
  Serial.print(digitalRead(27));
  Serial.print("\t");
  Serial.print(digitalRead(28));
  Serial.print("\t");
  Serial.print(digitalRead(29));
  Serial.print("\t");

  Serial.print(digitalRead(30));
  Serial.print("\t");
  Serial.print(digitalRead(31));
  Serial.print("\t");
  Serial.print(digitalRead(32));
  Serial.print("\t");
  Serial.print(digitalRead(33));
  Serial.print("\t");
  Serial.print(digitalRead(34));
  Serial.print("\t");
  Serial.print(digitalRead(35)); //
  Serial.print("\t");

  Serial.print(digitalRead(36));
  Serial.print("\t");
  Serial.print(digitalRead(37));
  Serial.print("\t");
  Serial.print(digitalRead(38)); //
  Serial.print("\t");
  Serial.print(digitalRead(39)); //
  Serial.print("\t");
  Serial.print(digitalRead(40));
  Serial.print("\t");
  Serial.print(digitalRead(41));
  Serial.print("\t");

  Serial.print(digitalRead(42));
  Serial.print("\t");
  Serial.print(digitalRead(43));
  Serial.print("\t");
  Serial.print(digitalRead(44));
  Serial.print("\t");
  Serial.print(digitalRead(45));
  Serial.print("\t");
  Serial.print(digitalRead(46));
  Serial.print("\t");
  Serial.println(digitalRead(47));
  delay(50);
}
