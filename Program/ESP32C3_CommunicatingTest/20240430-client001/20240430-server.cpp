// *  参考
// *  ・ https://monomonotech.jp/kurage/webbluetooth/ble_guide.html
// *  ・ https://www.musen-connect.co.jp/blog/course/trial-production/ble-beginner-1/


//サーバー側ソース

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// *  変数名の前についている"p" : ハンガリー記法によって変数名に追加された文字 この場合、その変数がポインタであることを示しているが、現在ではあまり使われていない

BLEServer *pServer = NULL;              // *  setup() 内で再度定義されるため消しても動く？
BLECharacteristic *pTxCharacteristic;   // *  MISO
bool deviceConnected = false;           // *  サーバーとの接続に成功したか = サーバーと接続されているかのフラグ
bool oldDeviceConnected = false;        // *  

// *  UUID自動生成サイト
// See the following for generating UUIDs:
// https://www.uuidgenerator.net/


// *  BLEについて

// *  Central(セントラル)       ≒ マスター 基本的により高性能な機器が担当
// *  Peripheral(ペリフェラル)  ≒ スレーブ 低性能 

// *  GATT通信    セントラル - ペリフェラル間の 1対1 の通信のこと

// *  Advertise   ペリフェラル機器が自身の存在を周囲に発信する無線信号のこと
// *              この信号をセントラル機器がスキャンし受信することで近くにあるペリフェラル機器を知ることができる
// *  RSSI        セントラル機器が受診したアドバタイズ信号の強さ
// *  Scan        セントラル機器が周囲のペリフェラルを探すこと
// *  Connect     セントラルが特定のペリフェラルと接続する処理のこと
// *  Disconnect  セントラルが接続を解除すること

// *  データ構造
// *  Service           キャラクタリスティックを機能単位でまとめたラベルのこと
// *  └ Characteristic  ペリフェラル機器がセントラル機器と共有するデータ構造のこと  このキャラクタリスティックを用いてデータのやり取りを行う
// *                    キャラクタリスティックには "Write", "Read", "Notify=キャラクタリスティックの値の変更を通知, セントラルはNotifyを受け取ってから値を読み出す" などの属性があり、それぞれの属性に応じて可能な操作が決まっている

// *    Characteristic
// *    ├ Value       やり取りする値そのもの
// *    ├ Property    キャラクタリスティックの属性を表す属性 (Write, Read, Notify...)
// *    └ Descriptor  Value の型や単位などの追加情報・バリューが変更された時にクライアントへ通知

// *  ServiceDiscovery(サービスディスカバリ)  接続処理時にペリフェラル機器のサービスを調べること

// *  これらサービス・キャラクタリスティックにはUUIDが決められている
// *  サーバー・クライアント TX・RX での表記は直感的に分かりづらいため、セントラル・ペリフェラル MISO・MOSI などに変更したい


//serverとcliantでTX,RXは逆転するのに注意
#define SERVICE_UUID "ee64a0c8-6995-11ed-a1eb-0242ac120002"             // *  サービスのUUID
#define CHARACTERISTIC_UUID_RX "ee64a49c-6995-11ed-a1eb-0242ac120002"   // *  受信用キャラクタリスティックのUUID
#define CHARACTERISTIC_UUID_TX "ee64a690-6995-11ed-a1eb-0242ac120002"   // *  送信用キャラクタリスティックのUUID

#define SERVER_NAME "esp32ble"    // *  サーバー用ESP32C3の名前を定義
#define Tx_value 'M'              // *  クライアント側で送信する文字を定期
std::string rx_Data;              // *  受信したデータを格納する文字列


// *  BLEServerCallbacks クラスの継承クラス MyServerCallbacks を定義
// *  恐らく、接続に成功した場合のコールバック関数・接続が切断された場合のコールバック関数のみをオーバーライドし、
// *  他のコールバック関数の動作を温存している

// *  継承クラス : すでに定義されたクラスから派生して、追加のメンバ変数・メンバ関数を持たせたクラスのこと
// *  　　　　　   元のクラス( ≒設計図 )から必要な部分だけを上書きして定義できるため、メンテナンス性・可読性が高いなどの利点がある

// *  コールバック関数 : 任意の関数Aに引数として渡され、関数A内で実行するための関数

// *  オーバーライド : 任意のクラスを継承したクラスを定義し、
// *  　　　　　　　   その中で既存のメンバ関数の動作を上書きすること

// *  似た言葉 オーバーライド : 同名で引数の型・数の異なる関数を、引数から推論して決定すること

class MyServerCallbacks : public BLEServerCallbacks {
  // *  接続に成功した場合のコールバック関数
  void onConnect(BLEServer *pServer) {
    deviceConnected = true;
  };
  
  // *  接続が切断された場合のコールバック関数
  void onDisconnect(BLEServer *pServer) {
    deviceConnected = false;
  }
};

// *  クラス MyCallbacks を作成し、コールバック関数をオーバーライドしている
class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();  // *  バリューを取得
      if (rxValue.length() > 0) {                         // *  バリューの長さが0ではない = バリューが送られた場合
        for (int i = 0; i < rxValue.length(); i++)        
          rx_Data=rx_Data+rxValue[i];                     // *  バリューの長さ分だけ rx_Data へ 受信データを読み出す = rxValue → rx_Data へコピー
      }
    }
};
void setup() {
  // *  シリアル通信を開始
  Serial.begin(115200);
  Serial.println("BLE_Server_start");

  // *  SERVER_NAME ("esp32ble") でBLEデバイスを作成 外部から認識するためのもの？
  // Create the BLE Device
  BLEDevice::init(SERVER_NAME);

  // *  サーバーを作成 BLEに関するありとあらゆる処理の起点？
  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // *  サービスを作成
  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // *  pService内に、 ペリフェラル → セントラルの送信用キャラクタリスティックを作成 ≒ MISO
  // *  第一引数に UUID 、第二引数に 属性 を指定
  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(
										CHARACTERISTIC_UUID_TX,
										BLECharacteristic::PROPERTY_NOTIFY
									);

  // *  送信用のデスクリプタを指定  ここで std::string などを指定すれば恐らく文字列を送信できる？
  pTxCharacteristic->addDescriptor(new BLE2902());

  // *  セントラル → ペリフェラルの受信用キャラクタリスティックを作成 ≒ MOSI
  // *  Txと同様に引数を指定
  BLECharacteristic * pRxCharacteristic = pService->createCharacteristic(
                                             CHARACTERISTIC_UUID_RX,
                                            BLECharacteristic::PROPERTY_WRITE
                                        );
  // *  受信用キャラクタリスティックのコールバック関数を指定
  // *  受信した際に呼ばれる
  pRxCharacteristic->setCallbacks(new MyCallbacks());                                      

  // *  サービスを開始
  // Start the service
  pService->start();
  
  // *  スキャンを開始
  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {

  // *  接続されている状態
  if (deviceConnected) {
    // *  送信する文字列を出力
    Serial.printf("*** NOTIFY: %c ***\n", Tx_value);
    
    char buffer[10];                      // *  送信用のバッファ
    sprintf(buffer, "%c", Tx_value);      // *  バッファに送信用文字列をコピー
    pTxCharacteristic->setValue(buffer);  // *  バッファの内容を送信？
    pTxCharacteristic->notify();          // *  送信用キャラクタリスティックの値の変更を通知
    Serial.printf("%s",rx_Data.c_str());  // *  受信したデータを出力  受信に関する云々をコールバック関数を指定すればライブラリ側で勝手にやってくれる？
    delay(1000);
  }
  // *  切断
  // disconnecting
  if (!deviceConnected && oldDeviceConnected) {   // *  セントラルと接続されてない かつ さっきまでセントラルと接続されていた = セントラルと切断されてすぐの状態
      delay(500);                                 // give the bluetooth stack the chance to get things ready
      // *  切断されたためアドバタイズを再開
      pServer->startAdvertising();                // restart advertising
      Serial.println("start advertising");
      oldDeviceConnected = deviceConnected;       // *  切断された場合、oldDeviceConnected に deviceConnected(false?) を代入する
  }

  // *  接続中にする処理がないなら書かなくて良い気がする...
  // connecting
  if (deviceConnected && !oldDeviceConnected) {   // *  デバイスが接続されている かつ さっきまでデバイスと接続されていなかった
      // do stuff here on connecting
      oldDeviceConnected = deviceConnected;       // *  接続中の場合、oldDeviceConnected に deviceConnecter(true?) を代入する
  }
}