#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  pinMode( 2, OUTPUT);
  pinMode(14, OUTPUT);

  // Sign for the end of bluetooth setup.
  for (int i=0; i<3; i++) {
    digitalWrite(2, HIGH);
    delay(50);
    digitalWrite(2, LOW);
    delay(50);
  }
}

void loop() {
  if (SerialBT.available()) {
    if (SerialBT.read() == 'T') {
        digitalWrite(14, HIGH);
    }
  }
  delay(20);
  digitalWrite(14, LOW);
}