HardwareSerial mySerial1(1);

void setup() {
  Serial.begin(9600);

  mySerial1.begin(115200,SERIAL_8N1,32,33);
  Serial.println("TOINIOT2 uart test esp32 2024-09-24");
}

void loop() {
  while(mySerial1.available()){
    char c = mySerial1.read();
    Serial.print(c);
  }

  mySerial1.println("ESP");

  delay(50);
}

// Atmega 2560
// 20 SCK line10
// 21 MOSI line11
// 22 MISO line12

// UNO
// 10 reset
// 11 MOSI
// 12 MISO
// 13 SCK