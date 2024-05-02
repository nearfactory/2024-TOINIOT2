// central = server = master

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define CENTRAL_NAME "IOT2_ESP32C3_CENTRAL"

#define SERVICE_UUID "17acb62a-e48e-4653-a2ed-7789f7e5ecc8"
#define COPI_UUID "6b9e155c-29e3-4cc7-a82c-67a941cf2a78"  // Central out Peripheral in
#define CIPO_UUID "7076c0f6-79bb-4fc9-bcea-57cba5163d55"

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

void loop() {
  if(device_connected){
    static std::string send_str = "message from central!";
    Serial.printf("notified : %s, ", send_str.c_str());
    COPI -> setValue(send_str);
    COPI -> notify();
    Serial.printf("recieved string from peripheral : \" %s \" \n", CIPO_string.c_str());
    delay(1000);
  }

  if(!device_connected && previous_device_connected){
    delay(500);
    server -> startAdvertising();
    Serial.println("\nbegan advertising\n");
    previous_device_connected = device_connected;
  }

  if(device_connected && !previous_device_connected) previous_device_connected = device_connected;
}