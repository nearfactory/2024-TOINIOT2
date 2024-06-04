<<<<<<< Updated upstream
String sendMessage;
String receivedMessage;
void setup() {
  Serial.begin(9600);    // Initialize the Serial monitor for debugging
  Serial2.begin(9600);   // Initialize Serial2 for sending data
}

void loop() {
  while (Serial2.available() > 0) {
    char receivedChar = Serial2.read();
    if (receivedChar == '\n') {
      Serial.println(receivedMessage);  // Print the received message in the Serial monitor
      receivedMessage = "";  // Reset the received message
    } else {
      receivedMessage += receivedChar;  // Append characters to the received message
    }
  }

  if (Serial.available() > 0) {
    char inputChar = Serial.read();
    if (inputChar == '\n') {
      Serial2.println(sendMessage);  // Send the message through Serial2 with a newline character
      sendMessage = "";  // Reset the message
    } else {
      sendMessage += inputChar;  // Append characters to the message
    }
  }
=======
/**********************************************************************
【ライセンスについて】
Copyright(c) 2022 by tamanegi
Released under the MIT license
'http://tamanegi.digick.jp/about-licence/

【マイコン基板】
 Teensy 4.1
 
【スケッチの説明】
 COMから読み取った情報を UART8へ出力します。
 UART8から読み取った情報を COMへ出力します。

【ライブラリ】
Teensy > Teensy 4.0

【準備】
 UARTの通信にFT232RLを使用します。

Teensy4.1(COM側) <-> FT232RL(UART2側)
 GPIO35(UART8 TX) <-> RX
 GPIO34(UART8 RX) <-> TX

 (Uart番号とオブジェクト名)
 COM - Serial
 UART1 - mySerial
 UART2 - Serial2
 UART3 - Serial3
 UART4 - Serial4
 UART5 - Serial5
 UART6 - Serial6
 UART7 - Serial7
 UART8 - mySerial
 
【バージョン情報】
2022/12/28 : 新規
**********************************************************************/
// #include <SoftwareSerial.h>

// SoftwareSerial mySerial(0,1);
void setup()
{
  // pinMode(2,INPUT_PULLUP);
  Serial.begin(9600);
  Serial1.begin(9600);
  // mySerial.setPins(2,3);
  // mySerial.setRX(2);
  // mySerial.setTX(3);
  // mySerial.begin(9600);
  // pinMode(0,INPUT_PULLUP);
  Serial.println("AtMega2560");
}

void loop()
{
  if(Serial1.available() != 0)          //UART5にデータがあれば、読み取った内容をUART0に送信
  {
      Serial.write(Serial1.read());
  }

  if(Serial.available() != 0)           //UART0にデータがあれば、読み取った内容をUART5に送信
  {
      Serial1.write(Serial.read());
  }

  // Serial.println(digitalRead(0));
  // delay(100);
>>>>>>> Stashed changes
}