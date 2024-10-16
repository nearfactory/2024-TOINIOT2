/*
#include <SoftwareSerial.h>

#define SOFT_RXD 19
#define SOFT_TXD 20
SoftwareSerial SoftSerial;

void setup() {
  SoftSerial.begin(115200, SWSERIAL_8N1, SOFT_RXD, SOFT_TXD, false, 256);
}

void loop() {
  SoftSerial.println("M5 Stamp Cam S3");
  delay(500);
}
*/

/*

RGBが正しく取得できない原因
→実験時にカメラの画素全体を特定の色で覆った状態でホワイトバランスが調整されてしまっため、起動直後しか正しい色を取得できなかった

*/

#include <vector>

#include <Arduino.h>
#include <esp_camera.h>

// GPIO19/GPIO20を使用するためsoftwareserialに変更
#include <SoftwareSerial.h>

#include "camera_pins.h"

// softwareserialのピンやボーレート
#define BAUDRATE 115200
#define SOFT_RXD 19
#define SOFT_TXD 20

#define WHITEBALANCE 16

#define QVGA_X 320
#define QVGA_Y 240

#define BUF_SPACE 10
#define BUF_X (QVGA_X + BUF_SPACE*2)
#define BUF_Y (QVGA_Y + BUF_SPACE*2)

SoftwareSerial SoftSerial;

camera_fb_t* Camera_fb;

struct Obj{
  uint32_t id=-1;
  uint32_t x=-1, y=-1, count=1;
};  

int8_t* buf = NULL;

void printBuf(){
  Serial.println("\n\n\n");

  for(int y=0;y<80;y++){
    for(int x=1;x<=BUF_X;x++){
      if(buf[x+y*3*BUF_X]==0){
        Serial.print(" ");
      }else if(buf[x+y*3*BUF_X]==1){
        Serial.print("B");
      }
      // Serial.print(buf[x+y*3*BUF_X]);
    }
    Serial.println();
  }

  Serial.println("\n\n\n");
}

void setup() {
  // SoftSerial.begin(BAUDRATE, SWSERIAL_8N1, SOFT_RXD, SOFT_TXD, false, 256);
  Serial.begin(BAUDRATE);

  delay(2000);

  // SoftSerial.println("Test Start");
  Serial.println("Test Start");

  // カメラ初期化
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;

  // 対応可能フォーマット
  // JPEG, RGB565
  config.pixel_format = PIXFORMAT_RGB565;

  // フレームサイズ
  // QVGA OK
  // VGA OK
  // SVGA たまに落ちる
  // XGA~ ×
  config.frame_size = FRAMESIZE_QVGA;
  // config.frame_size = FRAMESIZE_VGA; 
  // config.frame_size = FRAMESIZE_SVGA;
  // config.frame_size = FRAMESIZE_XGA;
  
  // config.jpeg_quality = 12;  // JPEG品質を調整（大→高品質）
  config.fb_count = 2;
  config.grab_mode = CAMERA_GRAB_LATEST;

  while (true) {
    if (esp_camera_init(&config) != ESP_OK) {
      // SoftSerial.println("カメラの初期化に失敗しました");
      Serial.println("カメラの初期化に失敗しました");
    } else {
      // SoftSerial.println("カメラが初期化されました");
      Serial.println("カメラが初期化されました");
      break;
    }
    delay(1000);
  }
  
  // カメラセンサーの取得
  sensor_t* s = esp_camera_sensor_get();
  if (s == NULL) {
    // SoftSerial.println("センサーの取得に失敗しました");
    Serial.println("センサーの取得に失敗しました");
    return;
  }

  // ホワイトバランスを自動に設定
  // ホワイトバランスをオンにするとRGBが正しく取得できないため、自動設定をオフに
  s->set_whitebal(s, 0);  // 0 = Off, 1 = On

  // 上下方向の反転の有無
  s->set_vflip(s, 0);  // 0 = 正常, 1 = 上下反転

  // 左右方向の反転の有無
  s->set_hmirror(s, 0);  // 0 = 正常, 1 = 左右反転

  // 他の設定例
  // s->set_brightness(s, 1);  // -2 to 2
  // s->set_contrast(s, 1);    // -2 to 2
  // s->set_saturation(s, 1);  // -2 to 2

  //16枚を空撮影、ホワイトバランス確保のため
  // for (int i = 0; i < WHITEBALANCE; i++) {
  //   // カメラから画像を取得
  //   Camera_fb = esp_camera_fb_get();
  //   if (!Camera_fb) {
  //     // SoftSerial.println("画像の取得に失敗しました");
  //     Serial.println("画像の取得に失敗しました");
  //     delay(1000);
  //     continue;
  //   }
  //   esp_camera_fb_return(Camera_fb);  // フレームバッファを解放
  // }
  


  Serial.println("\n\n\nmem");
  buf = reinterpret_cast<int8_t*>(malloc(sizeof(int8_t)*BUF_X*BUF_Y)); // しきい値を超えたピクセルのバッファ
  // memset(buf, 0, sizeof(int8_t)*BUF_X*BUF_Y);
  for(int i=0;i<BUF_X*BUF_Y;i++){
    buf[i] = 0;
  }
  printBuf();
  Serial.printf("%08d", buf);
  Serial.println("\n\n");
  if(buf == NULL){
    Serial.println("buf err!");
    return;
  } 

  delay(100);

}

void loop() {
  while(!Serial.available()){}
  while( Serial.available()) Serial.read();

  // カメラから画像を取得
  Camera_fb = esp_camera_fb_get();
  if (!Camera_fb) {
    // SoftSerial.println("画像の取得に失敗しました");
    Serial.println("画像の取得に失敗しました");
    return;
  }


  std::vector<Obj> obj(0);

  // しきい値を超えたピクセルのバッファを作る
  uint8_t threshold = 24;
  for(int i=0;i<Camera_fb->len/2;i++){
    uint16_t pixel = (Camera_fb->buf[i*2]<<8) + (Camera_fb->buf[i*2+1]);
    uint8_t r = (pixel & 0b1111100000000000) >> 11;
    uint8_t g = (pixel & 0b0000011111100000) >> 6;
    uint8_t b = (pixel & 0b0000000000011111);

    // 二値化
    if(b > threshold){
      buf[i+1+BUF_X + BUF_SPACE*2*i/QVGA_X] = 1;
    }else{
      buf[i+1+BUF_X + BUF_SPACE*2*i/QVGA_X] = 0;
    }
  }

  printBuf();

  // ノイズ除去 (膨張・収縮)
  int REPEAT = 1;
  for(int i=0;i<REPEAT;i++){
    // 膨張
    for(int y=1;y<=QVGA_Y;y++){
      for(int x=1;x<=QVGA_X;x++){
        int i=x+BUF_X*y;
        if(!buf[i]){
          // if(buf[i-BUF_X-1]||buf[i-BUF_X]||buf[i-BUF_X+1] || buf[i-1]||buf[i+1] || buf[i+BUF_X-1]||buf[i+BUF_X]||buf[i+BUF_X+1])
          if(buf[i-1])
          {
            buf[i] = 1;
          }
        }
      }
    }
  }

  printBuf();

  // 物体認識
  // https://www.etcnotes.info/almath/algofill.html
  uint32_t next_id = 0;
  for(int y=1;y<=QVGA_Y;y++){
    for(int x=1;x<=QVGA_X;x++){
      int i = x+BUF_X*y;
      if(buf[i] == 1){
        // Serial.printf("fill x:%d  y:%d\n", x, y);
        // fill(x,y,next_id);
        // buf[i] = next_id;
        next_id++;
      }
    }
  }
  
  Serial.printf("id:%d  ", next_id-1);

  uint16_t main = (Camera_fb->buf[76800]<<8) + (Camera_fb->buf[76801]);
  Serial.print(" Min:");
  Serial.print(0);
  Serial.print(" Max:");
  Serial.print(31);
  Serial.printf(" R:%02d", (main >> 11));
  Serial.printf(" G:%02d", (main & 0b0000011111100000) >> 6);
  Serial.printf(" B:%02d", main & 0b0000000000011111);
  
  esp_camera_fb_return(Camera_fb);

  Serial.println();
}