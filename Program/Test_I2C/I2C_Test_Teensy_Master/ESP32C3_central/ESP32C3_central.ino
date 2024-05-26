#include<Arduino.h>

#include<BLEDevice.h>
#include<BLEServer.h>
#include<BLEUtils.h>
#include<BLE2902.h>

#define CENTRAL_NAME "IOT2_ESP32C3_CENTRAL_TEST"

#define SERVICE_UUID "ae71ac5b-6be7-44b6-baf1-3fc57c4fc35e"
#define CIPO_UUID "aed5a97a-473a-4b5b-8b19-1d63bc1d12e1"
#define COPI_UUID "f3784b6c-3300-4221-918a-40f3a2bc63b2"

bool device_connected = false;
bool previous_device_connected = false;
std::string CIPO_string;
BLEServer *server;
BLEServer *Central;
BLECharacteristic *COPI = nullptr;
BLECharacteristic *CIPO = nullptr;

// BLE接続確立時のコールバック関数
class CentralCallbacks : public BLEServerCallbacks{
  void onConnect(BLEServer *server_){
    device_connected = true;
  }

  void  onDisconnect(BLEServer * server_){
    device_connected = false;
  }
};

class CharacteristicCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *characteristic_){
    std::string CIPO_string_ = characteristic_ -> getValue();
    if(CIPO_string_.length() > 0)
      CIPO_string = CIPO_string_;
  }
};

void setup() {
  Serial.begin(9600);
  Serial.println("BLE central began!\n\n");

  //BLE
  BLEDevice::init(CENTRAL_NAME);

  server = BLEDevice::createServer();
  server -> setCallbacks(new CentralCallbacks());

  BLEService *service = server -> createService(SERVICE_UUID);

  CIPO = service -> createCharacteristic(CIPO_UUID, BLECharacteristic::PROPERTY_NOTIFY);
  CIPO -> addDescriptor(new BLE2902());

  COPI = service -> createCharacteristic(COPI_UUID, BLECharacteristic::PROPERTY_WRITE);
  COPI -> setCallbacks(new CharacteristicCallbacks());

  service -> start();

  server -> getAdvertising() -> start();
  Serial.println("Waiting a client connection to notify...");
}

static std::string send_str = "message from central!";
static int count=0;
void loop() {
  if(device_connected){
    send_str = "central_" + std::to_string(count) ;
    count++;
    Serial.printf( "COPI : \" %s \" ,    CIPO : \" %s \" \n", send_str.c_str(), CIPO_string.c_str() );
    COPI -> setValue(send_str);
    COPI -> notify();
    delay(250);
  }

  if(!device_connected && previous_device_connected){
    delay(500);
    server -> startAdvertising();
    Serial.println("\nbegan advertising\n");
    previous_device_connected = device_connected;
  }

  if(device_connected && !previous_device_connected) previous_device_connected = device_connected;
}