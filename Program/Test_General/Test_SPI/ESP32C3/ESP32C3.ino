#include <ESP32DMASPISlave.h>

ESP32DMASPI::Slave slave;

static const int MSG_SIZE = 20;
uint8_t* s_message_buf;
uint8_t* r_message_buf;
int checksum;

void setup()
{
  Serial.begin(115200);
  delay(500);//シリアルの開始を待ち安定化させるためのディレイ（要調整）

  Serial.println("SPI Slave Start.");//シリアルモニタの確認用。

  // DMAバッファを使う設定　これを使うと一度に送受信できるデータ量を増やせる
  s_message_buf = slave.allocDMABuffer(MSG_SIZE); //DMAバッファを使う
  r_message_buf = slave.allocDMABuffer(MSG_SIZE); //DMAバッファを使う

  // 送受信バッファをリセット
  memset(s_message_buf, 0, MSG_SIZE);
  memset(r_message_buf, 0, MSG_SIZE);

  //送信データを作成してセット
  checksum = 0;
  for (int i = 0; i < MSG_SIZE - 1; i++) //配列の末尾以外をデータを入れる
  {
    uint8_t rnd = random(0, 255);
    s_message_buf[i] = rnd;
    checksum += rnd; //チェックサムを加算
  }
  s_message_buf[MSG_SIZE - 1] = uint8_t(checksum & 0xFF ^ 0xFF); //データ末尾にチェックサムにする

  slave.setDataMode(SPI_MODE3);
  slave.setMaxTransferSize(MSG_SIZE);
  slave.setDMAChannel(2); // 専用メモリの割り当て（1か2のみ)
  slave.setQueueSize(1); // キューサイズ　とりあえず1
  // HSPI(SPI2) のデフォルトピン番号は CS: 15, CLK: 14, MOSI: 13, MISO: 12
  slave.begin(); // 引数を指定しなければデフォルトのSPI（SPI2,HSPIを利用）
}

void loop()
{

  // キューが送信済みであればセットされた送信データを送信する。
  if (slave.remained() == 0) {
    slave.queue(r_message_buf, s_message_buf, MSG_SIZE);
  }

  // マスターからの送信が終了すると、slave.available()は送信サイズを返し、
  // バッファも自動更新される
  while (slave.available())
  {
    Serial.print(" Send : ");
    for (uint32_t i = 0; i < MSG_SIZE; i++)
    {
      Serial.print(s_message_buf[i]);
      Serial.print(",");
    }
    Serial.println();

    // show received data
    Serial.print(" Rsvd : ");
    for (size_t i = 0; i < MSG_SIZE; ++i) {
      Serial.print(r_message_buf[i]);
      Serial.print(",");
    }
    Serial.println();
    slave.pop();//トランザクションを終了するコマンドらしい

    //受信データのチェックサム確認
    checksum = 0;
    for (int i = 0; i < MSG_SIZE - 1 ; i++) { //受信データの末尾-1番までの値を合計
      checksum += int(r_message_buf[i]);
    }
    checksum = (checksum ^ 0xff) & 0xff; //合計値を反転し、下位2バイトを取得
      Serial.print(" cksum: "); Serial.println(uint8_t (r_message_buf[MSG_SIZE - 1]));

    
    if (uint8_t (checksum) == uint8_t(r_message_buf[MSG_SIZE - 1])) {
      Serial.print("   OK!: "); Serial.println(uint8_t (checksum));
    } else {
      Serial.print("**ERR*: "); Serial.println(uint8_t (checksum));
    }
    Serial.println();
  }
}


