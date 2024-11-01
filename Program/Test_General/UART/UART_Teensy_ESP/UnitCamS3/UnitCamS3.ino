#include <SoftwareSerial.h>

char buf[100];

SoftwareSerial mySerial1;

void setup() {
  // mySerial1.set
  for(int i=0;i<100;i++) buf[i] = '\0';
  mySerial1.begin(9600, SWSERIAL_8N1, 19, 20, false, 256);
}

void loop() {
  if(mySerial1.available()){
    for(int i=0;i<100;i++) buf[i] = '\0';
    int i=0;
    while(mySerial1.available()){
      buf[i] = (char)mySerial1.read();
      i++;
    }
  }

  // mySerial1.print("a");
  mySerial1.print(buf);
  delay(50);
}