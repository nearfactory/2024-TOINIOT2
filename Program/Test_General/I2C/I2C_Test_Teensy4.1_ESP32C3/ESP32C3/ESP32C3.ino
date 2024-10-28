// Wire Master Reader
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Reads data from an I2C/TWI slave device
// Refer to the "Wire Slave Sender" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

int led = 0;

void setup()
{
  pinMode(led, OUTPUT);
  Wire.begin();             // join i2c bus (address optional for master)
  Serial.begin(9600);       // start serial for output
}

void loop()
{
  Serial.print("read: ");

  //正しい値の送信には成功した(Teensyのシリアル通信で確認済み)
  //ここの256を512とかに変えると動かない(文字列だとせいぜいGyroまでしかデータを送れない)
  // → 地磁気の値のデータを圧縮した上で、1ビットずつバッファに書き込んだものを文字列として送信→ESP側で文字列に戻すという手法を試す
  //受信した文字列の一部がときどき文字化けしている
  //受信したデータが高頻度で「P.」とういう謎の文字列になる
  // → 発生する条件・原因を特定する必要がある
  digitalWrite(led, HIGH);  // briefly flash the LED
  Wire.requestFrom(8, 256);   // request 6 bytes from slave device #8

  while(Wire.available()) { // slave may send less than requested
    char c = Wire.read();   // receive a byte as character
    Serial.print(c);        // print the character
  }

  Serial.println();
  digitalWrite(led, LOW);
  delay(50);
}/*6-*/