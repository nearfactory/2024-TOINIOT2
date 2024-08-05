//clian側ソース
#include "BLEDevice.h"

#define SERVER_NAME         "TOINIOT2_ATACKER"
#define SERVICE_UUID "6a10c527-bac9-485e-bdb8-462f3b72bd59"
#define CHARACTERISTIC_UUID_TX "d4b0e22e-2597-48e3-886e-ea984f3f7db0"
#define CHARACTERISTIC_UUID_RX "2234bf79-5ff8-4b47-a139-b9a20801eaf4"
#define Tx_value 'S'

static BLEUUID  serviceUUID(SERVICE_UUID);
static BLEUUID  charUUID_RX(CHARACTERISTIC_UUID_RX);
static BLEUUID  charUUID_TX(CHARACTERISTIC_UUID_TX);

static BLEAddress *pServerAddress;
static boolean doConnect = false;
static boolean connected = false;
static BLERemoteCharacteristic* pRemoteCharacteristicRX;
static BLERemoteCharacteristic* pRemoteCharacteristicTX;

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
    Serial.print("Notify callback for characteristic ");
    Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
    Serial.print(" of data length ");
    Serial.println(length);
    Serial.print("data: ");
    Serial.println((char*)pData);
}

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
  }

  void onDisconnect(BLEClient* pclient) {
    connected = false;
    Serial.println("onDisconnect");
  }
};

bool connectToServer(BLEAddress pAddress) {
    Serial.print("Forming a connection to ");
    Serial.println(pAddress.toString().c_str());
    BLEClient*  pClient  = BLEDevice::createClient();
    Serial.println(" - Created client");

    pClient->setClientCallbacks(new MyClientCallback());
    
    // Connect to the remove BLE Server.            
    pClient->connect(pAddress);
    Serial.println(" - Connected to server");
    pClient->setMTU(517); //set client to request maximum MTU from server (default is 23 otherwise)

    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    Serial.print(" - Connected to server :");
    Serial.println(serviceUUID.toString().c_str());
    if (pRemoteService == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(serviceUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    
    pRemoteCharacteristicRX = pRemoteService->getCharacteristic(charUUID_RX);
    if (pRemoteCharacteristicRX == nullptr) {
      Serial.print("Failed to find characteristic RX ID: ");
      Serial.println(charUUID_RX.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.print(" - Found characteristic RX ID: ");
    Serial.println(charUUID_RX.toString().c_str());

    // Send用キャラクタリスティックの参照を取得する
    pRemoteCharacteristicTX = pRemoteService->getCharacteristic(charUUID_TX);
    if (pRemoteCharacteristicTX == nullptr) {
      Serial.print("Failed to find characteristic TX ID ");
      Serial.println(charUUID_TX.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.print(" - Found characteristic TX ID: ");
    Serial.println(charUUID_TX.toString().c_str());
    
    // Read the value of the characteristic.
    if(pRemoteCharacteristicRX->canRead()) {
      String value = pRemoteCharacteristicRX->readValue();
      Serial.print("The characteristic value was: ");
      Serial.println(value.c_str());
    }

      Serial.println("\n\n\n\n\n\n\n\n");
      if(pRemoteCharacteristicTX->canWrite()) Serial.println("can write");
      if(pRemoteCharacteristicTX->canNotify()) Serial.println("can notify");
      if(pRemoteCharacteristicTX->canRead()) Serial.println("can read");
      Serial.println("\n\n\n\n\n\n\n\n\n");
      delay(1000);
    //pRemoteCharacteristicRX->registerForNotify(notifyCallback);
     // Serial.println("End of notify");
          return true;
}


class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());
    Serial.println(advertisedDevice.getName().c_str());
    
    if(advertisedDevice.getName()==SERVER_NAME){
      Serial.println(advertisedDevice.getAddress().toString().c_str());
      advertisedDevice.getScan()->stop();
      pServerAddress = new BLEAddress(advertisedDevice.getAddress());
      doConnect = true;
    }
  }
};

void get_scan(){
  BLEScan* pBLEScan = BLEDevice::getScan();
  Serial.println("getScan");
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  Serial.println("setAdvertisedDeviceCallbacks");
  pBLEScan->setActiveScan(true);
  pBLEScan->start(10); 
  Serial.println("");
  Serial.println("End of scan");
}


void setup() {
  Serial.begin(115200);
  Serial.println("BLE_Cliant_start");
  BLEDevice::init("TOINIOT2_DEFENDER");
  get_scan();
  Serial.println("End of setup");
}


void loop() {
  if (doConnect == true) {
    if (connectToServer(*pServerAddress)) {
      Serial.println("We are now connected to the BLE Server.");
      connected = true;
    } else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
      connected = false;
    }
    doConnect = false;
  }
  if (connected) {
    String value = pRemoteCharacteristicRX->readValue();
    String strVal = value.c_str();
    int strNum=strVal.toInt();
    Serial.println(strVal);
    pRemoteCharacteristicRX -> writeValue("periferal", strlen("periferal"));
    // pRemoteCharacteristicTX -> 
  } else{
    Serial.println("Not connected");
    doConnect = false;
    get_scan();
  }
  delay(1000);
}