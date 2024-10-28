//サーバー側ソース

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer *pServer = NULL;
BLECharacteristic *pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

//serverとcliantでTX,RXは逆転するのに注意
#define SERVICE_UUID "ee64a0c8-6995-11ed-a1eb-0242ac120002"
#define CHARACTERISTIC_UUID_RX "ee64a49c-6995-11ed-a1eb-0242ac120002"
#define CHARACTERISTIC_UUID_TX "ee64a690-6995-11ed-a1eb-0242ac120002"

#define SERVER_NAME "esp32ble"
#define Tx_value 'FUJIKI'
std::string rx_Data;

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) {
    deviceConnected = true;
  };

  void onDisconnect(BLEServer *pServer) {
    deviceConnected = false;
  }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();
      if (rxValue.length() > 0) {
        rx_Data="";
        for (int i = 0; i < rxValue.length(); i++)
          rx_Data=rx_Data+rxValue[i];
      }
    }
};
void setup() {
  Serial.begin(115200);
  Serial.println("BLE_Server_start");

  // Create the BLE Device
  BLEDevice::init(SERVER_NAME);

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(
										CHARACTERISTIC_UUID_TX,
										BLECharacteristic::PROPERTY_NOTIFY
									);
  pTxCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic * pRxCharacteristic = pService->createCharacteristic(
                                             CHARACTERISTIC_UUID_RX,
                                            BLECharacteristic::PROPERTY_WRITE
                                        );
  pRxCharacteristic->setCallbacks(new MyCallbacks());                                      

  // Start the service
  pService->start();
  
  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {

  if (deviceConnected) {
    Serial.printf("*** NOTIFY: %s ***\n", Tx_value);
    char buffer[10];
    sprintf(buffer, "%s", Tx_value);
    pTxCharacteristic->setValue(buffer);
    pTxCharacteristic->notify();
    Serial.printf("%s",rx_Data.c_str());
    delay(250);
  }
      // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
		// do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}