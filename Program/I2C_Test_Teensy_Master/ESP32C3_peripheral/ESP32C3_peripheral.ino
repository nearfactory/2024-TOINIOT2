//ESP32C3
//Slave, Peripheral

#include <Wire.h>

#include "BLEDevice.h"

#define CENTRAL_NAME "IOT2_ESP32C3_CENTRAL_TEST"

#define SERVICE_UUID "ae71ac5b-6be7-44b6-baf1-3fc57c4fc35e"
#define CIPO_UUID "aed5a97a-473a-4b5b-8b19-1d63bc1d12e1"
#define COPI_UUID "f3784b6c-3300-4221-918a-40f3a2bc63b2"


BLEUUID service_BLE_UUID(SERVICE_UUID);
BLEUUID COPI_BLE_UUID(COPI_UUID);
BLEUUID CIPO_BLE_UUID(CIPO_UUID);

char c=0;

bool do_connect = false;
bool connected = false;
BLEAddress *server_address = nullptr;
BLERemoteCharacteristic *remote_characteristic_CIPO;
BLERemoteCharacteristic *remote_characteristic_COPI;

constexpr uint8_t I2C_ADDR = 0x32;


void notifyCallback(
  BLERemoteCharacteristic *BLE_remote_characteristic_,
  uint8_t *data_,
  size_t length_,
  bool is_notify_
)
{
  Serial.print("Notify callback for characteristic ");
  Serial.print(BLE_remote_characteristic_->getUUID().toString().c_str());
  Serial.print(" of data length ");
  Serial.println(length_);
  Serial.print("data : ");
  Serial.println(reinterpret_cast<char *>(data_));
};

class clientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient *client_) {}

  void onDisconnect(BLEClient *client_) {
    connected = false;
    Serial.println("disconnected!");
  }
};

bool connectToServer(BLEAddress address_) {
  Serial.print("Forming a connection to ");
  Serial.println(address_.toString().c_str());
  BLEClient *client_ = BLEDevice::createClient();
  Serial.println(" - Created client!");

  client_->setClientCallbacks(new clientCallback());

  client_->connect(address_);
  Serial.println(" - Connected to server!");
  client_->setMTU(517);

  BLERemoteService *remote_service = client_->getService(service_BLE_UUID);
  Serial.print(" - Connected to server : ");
  Serial.println(service_BLE_UUID.toString().c_str());

  if (remote_service == nullptr) {
    Serial.print("Failed to find our service UUID : ");
    Serial.println(service_BLE_UUID.toString().c_str());
    client_->disconnect();

    return false;
  }

  remote_characteristic_COPI = remote_service->getCharacteristic(COPI_BLE_UUID);
  if (remote_characteristic_COPI == nullptr) {
    Serial.print("Failed to find characteristic COPI ID : ");
    Serial.println(COPI_BLE_UUID.toString().c_str());
    client_->disconnect();

    return false;
  }

  //この辺を全部CIPOにすると動いた
  remote_characteristic_CIPO = remote_service->getCharacteristic(COPI_BLE_UUID);
  if (remote_characteristic_CIPO == nullptr) {
    Serial.print("Failed to find characteristic CIPO ID : ");
    Serial.println(COPI_BLE_UUID.toString().c_str());
    client_->disconnect();

    return false;
  }

  Serial.print(" - Found characteristic CIPO ID : ");
  Serial.println(COPI_BLE_UUID.toString().c_str());

  if (remote_characteristic_CIPO->canRead()) {
    std::string recieved_str = remote_characteristic_CIPO->readValue();
    Serial.print("The characteristic value was : ");
    Serial.println(recieved_str.c_str());
  }

  if (remote_characteristic_CIPO->canNotify()) return true;
};

class advertisedDeviceCallback : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertised_device_) {
    Serial.print("BLE advertised device found : ");
    Serial.println(advertised_device_.toString().c_str());
    Serial.println(advertised_device_.getName().c_str());

    if (advertised_device_.getName() == CENTRAL_NAME) {
      Serial.println(advertised_device_.getAddress().toString().c_str());
      advertised_device_.getScan()->stop();
      server_address = new BLEAddress(advertised_device_.getAddress());

      do_connect = true;
    }
  }
};

void get_scan() {
  BLEScan *BLE_scan = BLEDevice::getScan();
  Serial.println("getScan");
  BLE_scan->setAdvertisedDeviceCallbacks(new advertisedDeviceCallback());
  Serial.println("set advertised device callbacks!");
  BLE_scan->setActiveScan(true);
  BLE_scan->start(10);
  Serial.println("");
  Serial.println("End of scan!");
};

void setup() {
  //Serial
  Serial.begin(9600);
  delay(1000);
  Serial.println("ESP32C3");
  //BLE
  BLEDevice::init("IOT2_ESP32C3_PERIPHERAL_TEST");
  get_scan();
  //I2C
  Wire.begin(I2C_ADDR);
  Wire.onReceive(recieveEvent);
}

std::string CIPO_string = "";

void loop() {
  if (do_connect) {
    if (connectToServer(*server_address)) {
      Serial.println("we are now connected to the BLE server.");
      connected = true;
    } else {
      Serial.println("we have failed to connect to the server; there is nothing more we will do.");
      connected = false;
    }
    do_connect = false;
  }

  if (connected) {
    std::string COPI_string = remote_characteristic_COPI->readValue();
    remote_characteristic_CIPO->writeValue(CIPO_string.c_str(), CIPO_string.length());
    Serial.printf("COPI : \" %s \" ,    CIPO : \" %s \"\n", COPI_string.c_str(), CIPO_string.c_str());
  } else {
    Serial.println("Not connected!");
    do_connect = false;
    get_scan();
  }
}

void recieveEvent(int number) {
  CIPO_string = "";
  while (Wire.available() > 0) {
    char c = Wire.read();
    CIPO_string += c;
  }

  return;
}