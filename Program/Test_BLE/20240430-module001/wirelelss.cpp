#include "wireless.h"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// ★★★
BLEServer *pServer = NULL;
BLECharacteristic *pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;

/*
受信・送信のデータ/値の使いわけについて

値：実際にBLEを使用してやり取りをするもの
データ：BLEで送られたデータを変換するなどして可用性を高めたもの

という使い分けをしています
*/

// 送信値
char txValue[];

// 受信データ
std::string rx_data;

// 接続したときにフラグ変数をtrueに変更
void MyServerCallbacks::onConnect(BLEServer *pServer)
{
  deviceConnected = true;
}

// 接続を切ったときにフラグ変数をfalseに変更
void MyServerCallbacks::onDisconnect(BLEServer *pServer)
{
  deviceConnected = false;
}

// ★★★
void MyCallbacks::onWrite(BLECharacteristic *pCharacteristic)
{
  // ★★★
  std::string rxValue = pCharacteristic->getValue();

  // もし値が送られてきていたら（受信値の長さが0より大きいなら）
  if (rxValue.length() > 0)
  {

    // 受信データを初期化
    rx_data = "";

    // 受信値の長さ分だけforで繰り返し取得し受信データの末尾に結合
    for (int i = 0; i < rxValue.length(); i++)
    {
      rx_data += rxValue[i];
    }
  }
}

void wirelessSetup()
{
  // BLEデバイスを作成
  BLEDevice::init(SERVER_NAME);

  // 作成したデバイスをサーバーとして設定
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // BLEサービスを作成
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // BLE特性を作成
  pTxCharacteristic = pService->createCharacteristic(
      CHAR_UUID_TX,
      BLECharacteristic::PROPERTY_NOTIFY);
  pTxCharacteristic->addDescriptor(new BLE2902);

  BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(
      CHAR_UUID_RX,
      BLECharacteristic::PROPERTY_WRITE);
  pRxCharacteristic->setCalllbacks(new MyCallbacks());

  // BLEサービスを開始
  pService->start();

  // 管理を開始
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}

void wirelessGetData()
{
  // もし接続が確立していたら
  if (deviceConnected)
  {
    // 送信値を出力
    Serial.printf("Send: %s\t", txValue);

    // 送信データのバッファ
    char buffer[10];

    // 送信値を文字列に変換して送信データのバッファに保存
    sprintf(buffer, "%s", txValue);

    // バッファを送信データにセット
    pTxCharacteristic->setValue(buffer);
    pTxCharacteristic->notify();

    //　受信データを出力
    Serial.printf("%s\n", rx_data.c_str());
  }

  //もし接続ができていなかったら
  if(!deviceConnected && oldDeviceConnected){
    pServer->startAdvertising();
    Serial.println("start advertising");
    oldDeviceConnected = deviceConnected;
  }
  //もし接続が復活したら
  if(deviceConnected && !oldDeviceConnected){
    olldDeviceConnected = deviceConnected;
  }
}