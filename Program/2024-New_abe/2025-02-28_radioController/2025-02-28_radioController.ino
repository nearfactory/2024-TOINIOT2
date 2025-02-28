#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define CENTRAL_NAME "TOINIOT2_ATACKER"
#define SERVICE_UUID "6a10c527-bac9-485e-bdb8-462f3b72bd59"
#define NOTIFY_UUID "07edec9a-716c-4379-b18b-2bb8eb1a2bea"
#define D2A_UUID "d4b0e22e-2597-48e3-886e-ea984f3f7db0"
#define A2D_UUID "2234bf79-5ff8-4b47-a139-b9a20801eaf4"

bool device_connected = true;
bool previous_device_connected = false;
std::string D2A_str;
BLEServer *central;
BLECharacteristic *NOTIFY = nullptr;
BLECharacteristic *D2A = nullptr;
BLECharacteristic *A2D = nullptr;


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
    String _D2A = _characteristic -> getValue();
    // _characteristic -> setValue();
    if(_D2A.length()>0){
      D2A_str = _D2A.c_str();
      // Serial1.println(D2A_str.c_str());
    }
      // Serial.println("\n\n\n\nreceived!\n\n\n\n");
  }
};

void setup() {
  Serial.begin(9600);
  Serial.println("BLE central(atacker)");

  // Serial1.setPins(D7,D6);
  // Serial1.begin(9600);

  BLEDevice::init(CENTRAL_NAME);

  central = BLEDevice::createServer();
  central -> setCallbacks(new CentralCallbacks());

  BLEService *service = central -> createService(SERVICE_UUID);

  NOTIFY = service -> createCharacteristic(NOTIFY_UUID, BLECharacteristic::PROPERTY_NOTIFY);
  NOTIFY -> addDescriptor(new BLE2902());

  D2A = service -> createCharacteristic(D2A_UUID, BLECharacteristic::PROPERTY_WRITE);
  D2A -> addDescriptor(new BLE2902());

  A2D = service -> createCharacteristic(A2D_UUID, BLECharacteristic::PROPERTY_WRITE);
  A2D -> setCallbacks(new CharacteristicCallbacks());

  service -> start();

  central -> getAdvertising() -> start();
  Serial.println("waiting a connecting");
}

std::string A2D_str = "message from central!";
void loop() {
  static uint32_t count = 0;
  if(device_connected){
    A2D_str = "central_" + std::to_string(count) ;
    count++;
    Serial.printf( "A2D : \" %s \" ,    D2A : \" %s \" \n", A2D_str.c_str(), D2A_str.c_str() );
    A2D -> setValue(A2D_str.c_str());
    A2D -> notify();
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