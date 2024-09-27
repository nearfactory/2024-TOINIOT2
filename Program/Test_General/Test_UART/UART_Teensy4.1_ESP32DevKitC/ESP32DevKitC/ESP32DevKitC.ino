HardwareSerial mySerial1();
HardwareSerial mySerial2();
HardwareSerial mySerial3();

void setup() {
  mySerial1.begin(9600,SERIAL_8N1,32,33);
  mySerial2.begin(9600,SERIAL_8N1,32,33);
  mySerial3.begin(9600,SERIAL_8N1,32,33);
}

void loop() {
  

}
