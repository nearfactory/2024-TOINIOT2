// central(attacker)

#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define CENTRAL_NAME "TOINIOT2_CONTROLLER"
#define SERVICE_UUID "6a10c527-bac9-485e-bdb8-462f3b72bd59"
#define NOTIFY_UUID "07edec9a-716c-4379-b18b-2bb8eb1a2bea"
#define CIPO_UUID "d4b0e22e-2597-48e3-886e-ea984f3f7db0"
#define COPI_UUID "2234bf79-5ff8-4b47-a139-b9a20801eaf4"

bool device_connected = true;
bool previous_device_connected = false;
std::string CIPO_str;
BLEServer *central;
BLECharacteristic *NOTIFY = nullptr;
BLECharacteristic *CIPO = nullptr;
BLECharacteristic *COPI = nullptr;


class CentralCallbacks : public BLEServerCallbacks{
  void onConnect(BLEServer *_central){
    device_connected = true;
  }
  void onDisconnected(BLEServer *_central){
    device_connected = false;
  }
};

class CharacteristicCallbacks : public BLECharacteristicCallbacks{
  void onWrite(BLECharacteristic *_characteristic){
    String _CIPO = _characteristic -> getValue();
    if(_CIPO.length()>0){
      CIPO_str = _CIPO.c_str();
    }
  }
};

void setup() {
  Serial.begin(9600);
  Serial.println("TOINIOT2 Radio Controller");

  Serial1.setPins(D7,D6);
  Serial1.begin(9600);

  BLEDevice::init(CENTRAL_NAME);

  central = BLEDevice::createServer();
  central -> setCallbacks(new CentralCallbacks());

  BLEService *service = central -> createService(SERVICE_UUID);

  NOTIFY = service -> createCharacteristic(NOTIFY_UUID, BLECharacteristic::PROPERTY_NOTIFY);
  NOTIFY -> addDescriptor(new BLE2902());

  CIPO = service -> createCharacteristic(CIPO_UUID, BLECharacteristic::PROPERTY_WRITE);
  CIPO -> setCallbacks(new CharacteristicCallbacks());

  COPI = service -> createCharacteristic(COPI_UUID, BLECharacteristic::PROPERTY_READ);
  COPI -> addDescriptor(new BLE2902());

  service -> start();

  central -> getAdvertising() -> start();
}

std::string COPI_str = "message from central!";
void loop() {
  static uint32_t count = 0;
  if(device_connected){
    COPI_str = "central_" + std::to_string(count) ;
    count++;
    Serial.printf( "COPI : \" %s \" ,    CIPO : \" %s \" \n", COPI_str.c_str(), CIPO_str.c_str() );
    COPI -> setValue(COPI_str.c_str());
    COPI -> notify();
    delay(250);
  }

  if(!device_connected && previous_device_connected){
    delay(500);
    central -> startAdvertising();
    Serial.println("\nbegan advertising\n");
    previous_device_connected = device_connected;
  }

  if(device_connected && !previous_device_connected) previous_device_connected = device_connected;
}