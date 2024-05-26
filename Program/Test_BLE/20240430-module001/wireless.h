#ifndef WIRELESS_H
#define WIRELESS_H

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// 定数・関数の宣言
// サーバー名
extern constexpr char SERVER_NAME[] = "IOT2-S";
// UUID
extern constexpr char SERVICE_UUID[] = "ee64a0c8-6995-11ed-a1eb-0242ac120002";
extern constexpr char CHAR_UUID_RX[] = "ee64a49c-6995-11ed-a1eb-0242ac120002";
extern constexpr char CHAR_UUID_TX[] = "ee64a690-6995-11ed-a1eb-0242ac120002";

// 送信データ
extern char tx_data[];
// 受信データ
extern std::string rx_data;

// ★★★
BLEServer *pServer = NULL;
BLECharacteristic *pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;

class MyServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer);
    void onDisconnect(BLEServer);
};

class MyCallbacks : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic);
};

void wirelessSetup();
void wirelessGetData()

#endif