//client

// BLE通信に必要なライブラリをインクルード
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEClient.h>
#include <BLEServer.h>

// その他ライブラリをインクルード
#include <Arduino.h>
#include <Wire.h>

BLEClient*  pClient;
bool doconnect = false;

// サーバー側で設定したBLEサーバーの名前を設定
#define bleServerName "ESP32C3_IOT2_01"

//Address of the peripheral device. Address will be found during scanning...
static BLEAddress *pServerAddress;

// UUIDを定義
BLEUUID serviceUUID("181A"); // Environmental Sensing
BLEUUID charUUID("2A59");    // Analog Output

// 受信した値
char light_val[1024];

// 近くのBluetoothデバイスを検索し、指定したサーバーの名前と比較してキャプチャしてアドレスを取得
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {

    // サーバー名と同一の名前かどうかをチェック
    if (advertisedDevice.getName() == bleServerName) {

      // 必要なデータを取得し終えたらスキャンは終了することがある
      advertisedDevice.getScan()->stop();

      // 送信主のアドレスを取得する
      pServerAddress = new BLEAddress(advertisedDevice.getAddress());

      Serial.println("Device found. Connecting!");
    }
  }
};

void setup() {
  // シリアル通信を開始
  Serial.begin(115200);
  Serial.println("Starting BLE client...");

  // 受信側のデバイスを指定した名前で作成
  BLEDevice::init("ESP32C3_IOT2_02");

  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 30 seconds.
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(30);

  pClient = BLEDevice::createClient();
  // Connect to the remove BLE Server.
  pClient->connect(*pServerAddress);
  Serial.println(" - Connected to server");

  // サーバー側のUUIDを探す
  BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(serviceUUID.toString().c_str());
    return;
  }

  // サーバー側の特性のUUIDを探す
  BLERemoteCharacteristic* pCharacteristic = pRemoteService->getCharacteristic(charUUID);
  if (pCharacteristic == nullptr) {
    Serial.print("Failed to find our characteristic UUID");
    return;
  }
  Serial.println(" - Found light value characteristics");

  // 取得した2つのUUIDを使用して通信し、値を受診する
  pCharacteristic->registerForNotify([](BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
    Serial.println("Notify received");
    Serial.print("Value: ");
    Serial.println(*pData);
    snprintf(light_val, sizeof(light_val), "%d", *pData);
  });

  doconnect = true;
}

void loop() {
  if (doconnect) {
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    BLERemoteCharacteristic* pCharacteristic = pRemoteService->getCharacteristic(charUUID);
    pCharacteristic->registerForNotify([](BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
      Serial.println("Notify received");
      Serial.print("Value: ");
      Serial.println(*pData);
      snprintf(light_val, sizeof(light_val), "%d", *pData);
    });
  }
  Serial.println(light_val);
  delay(1000);
}