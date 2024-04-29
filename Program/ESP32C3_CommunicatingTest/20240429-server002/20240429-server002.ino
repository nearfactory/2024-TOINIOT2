//server

// BLE関連で必要なライブラリをインクルード

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <BLEServer.h>

// その他ライブラリをインクルード
#include <Arduino.h>
#include <Wire.h>

// BLEサーバーの名前を指定
#define bleServerName "ESP32C3_IOT2_01"

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;

// 送信する値を初期化
int light_val = 0;

class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  };
  
  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  }
};

void setup() {
  // シリアル通信を開始
  Serial.begin(115200);
  
  // 指定した名前でBLEデバイスを作成
  BLEDevice::init(bleServerName);

  // 作成したBLEデバイスをサーバーとして設定
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // 指定したUUIDを使用して特性プロパティを指定
  BLEService *pService = pServer->createService(BLEUUID((uint16_t)0x181A)); // Environmental Sensing
  pCharacteristic = pService->createCharacteristic(
    BLEUUID((uint16_t)0x2A59), // Analog Output
    
    // 継続的に値を送信するためには PROPERTY_NOTIFY　を指定すること
    BLECharacteristic::PROPERTY_NOTIFY
  );
  pCharacteristic->addDescriptor(new BLE2902());
  
  // BLE通信を開始
  pService->start();


  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(pService->getUUID());
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x0);
  pAdvertising->setMinPreferred(0x1F);
  BLEDevice::startAdvertising();
}

void loop() {
  if (deviceConnected) {
    // 送信する値を10msごとに更新
    pCharacteristic->setValue(light_val);
    pCharacteristic->notify();

    Serial.println(light_val);
    
    // 大量のパケットが送信されるとBLEが混雑してしまうため、それを防止する目的でdelay()を入れている
    delay(10);
  }
  light_val = light_val < 10000 ? light_val+1 : 0;
}