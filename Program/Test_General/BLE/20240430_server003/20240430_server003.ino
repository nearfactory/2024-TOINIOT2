#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

String MACadd = "24:62:AB:DC:30:5E";
uint8_t address[6]  = {0x24, 0x62, 0xAB, 0xDC, 0x30, 0x5E};
bool connected;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32test", true); 
  Serial.println("The device started in master mode, make sure remote BT device is on!");
  
  // connect(address) is fast (upto 10 secs max), connect(name) is slow (upto 30 secs max) as it needs
  // to resolve name to address first, but it allows to connect to different devices with the same name.
  // Set CoreDebugLevel to Info to view devices bluetooth address and device names
  connected = SerialBT.connect(address);
  
  if(connected) {
    Serial.println("Connected Succesfully!");
  } else {
    while(!SerialBT.connected(10000)) {
      Serial.println("Failed to connect. Make sure remote device is available and in range, then restart app."); 
    }
  }
  // disconnect() may take upto 10 secs max
  if (SerialBT.disconnect()) {
    Serial.println("Disconnected Succesfully!");
  }
  // this would reconnect to the name(will use address, if resolved) or address used with connect(name/address).
  SerialBT.connect();

  pinMode( 2, OUTPUT);
  pinMode(14, INPUT_PULLUP);

  // Sign for the end of bluetooth setup.
  for (int i=0; i<3; i++) {
    digitalWrite(2, HIGH);
    delay(50);
    digitalWrite(2, LOW);
    delay(50);
  }
}

void loop() {
  if (digitalRead(14) == LOW) {
    Serial.println("LED is ON.");
    SerialBT.write('T');
  }
  delay(20);
}