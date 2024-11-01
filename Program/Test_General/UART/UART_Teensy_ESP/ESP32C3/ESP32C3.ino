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
 UART1 - Serial1
 UART2 - Serial2
 UART3 - Serial3
 UART4 - Serial4
 UART5 - Serial5
 UART6 - Serial6
 UART7 - Serial7
 UART8 - Serial1
 
【バージョン情報】
2022/12/28 : 新規
**********************************************************************/

void setup()
{
  Serial.begin(9600);
  Serial1.setPins(D7,D6);
  Serial1.begin(9600);
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
}