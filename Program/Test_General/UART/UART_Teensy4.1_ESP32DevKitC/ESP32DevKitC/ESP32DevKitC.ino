HardwareSerial mySerial1();
HardwareSerial mySerial2();
HardwareSerial mySerial3();

void setup() {
  mySerial1.begin(9600,SERIAL_8N1,32,33);
  mySerial2.begin(9600,SERIAL_8N1,32,33);
  mySerial3.begin(9600,SERIAL_8N1,32,33);
}

void loop() {
  while(mySerial1().available()){
    Serial.print(mySerial1.read());
  }
  while(mySerial2().available()){
    Serial.print(mySerial2.read());
  }
  while(mySerial3().available()){
    Serial.print(mySerial3.read());
  }

  mySerial1.println("mySerial1");
  mySerial2.println("mySerial2");
  mySerial3.println("mySerial3");
}
