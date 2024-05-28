// Teensy4.1

//Programing by たまねぎ
//【スケッチの説明】
//RP2040 CPU搭載基板で使用できます。
//
//Raspberry Pi pico 同士でUART通信を行います。
//◆◆本スケッチは送信側、受信側共通です。◆◆
//
//パスコンからの読み取り文字を受信側にスルーします。
//送信側から読み取った文字をパソコンへスルーします。
//
//【準備】
//・Raspberry Pi pico を2枚用意します。
//UARTはUART0を使用します。
//送信側のRaspberry Pi pico のUART0 TX にGP0, UART0 RXにGP1を使用します。
//受信側のRaspberry Pi pico には、送信側の配線がクロスするように配線します。
//
//Raspberry Pi pico の送信側と受信側に本スケッチを書き込みます。
//
//【バージョン情報】
// 2022/7/9 : 新規

#define UART0_TX   1               //SDAにはGP0を使用する
#define UART0_RX   0               //SCLにはGP1を使用する

uint32_t i = 0;

void setup() 
{

  Serial.begin(115200);           //パソコン側からの読み取りと書き込みに使用します。

  Serial1.setTX(UART0_TX);        //もう一方のRaspberry Pi Picoとの通信に使用します。
  Serial1.setRX(UART0_RX);
  Serial1.begin(115200);
}

void loop() 
{
  char buf = 0;

  //<<送信側としての処理>>
  //パソコンからの受信処理と受信側への送信
  if(Serial.available() == true)
  {
    buf = Serial.read();

    //ASCコード上で、0b1000 0000ビットが0の時、0b0111 0000 のいずれかが 1なら英文字（正確ではありません。)
    if(((buf & 0x80) != 0x80) && ((buf & 0x40) == 0x40))
    {
      //ASCコード上で 0b0010 0000 のビットを反転させることで、大文字と小文字を入れ替える。
      buf = buf ^ 0x20;
    }
    //受信側へデータを送信する。
    Serial1.write(buf);
  }  

  //<<受信側としての処理>>
  //送信側からの受信処理と、パソコンへの送信
  if(Serial1.available() == true)
  {
    buf = Serial1.read();
    Serial.write(buf);
  } 
}