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

#include <Arduino.h>
#include <esp_camera.h>

// GPIO19/GPIO20を使用するためsoftwareserialに変更
#include <SoftwareSerial.h>

#include "camera_pins.h"

// softwareserialのピンやボーレート
#define BAURATE 115200
#define SOFT_RXD 19
#define SOFT_TXD 20


SoftwareSerial SoftSerial;

camera_fb_t* Camera_fb;

void setup() {
  // SoftSerial.begin(BAURATE, SWSERIAL_8N1, SOFT_RXD, SOFT_TXD, false, 256);
  Serial.begin(BAURATE);

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
  // JPEG
  // RGB565
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
  
  config.jpeg_quality = 12;  // JPEG品質を調整（大→高品質）
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

  // SoftSerial.println("Test Start");
  Serial.println("Test Start");
  
  // カメラセンサーの取得
  sensor_t* s = esp_camera_sensor_get();
  if (s == NULL) {
    // SoftSerial.println("センサーの取得に失敗しました");
    Serial.println("センサーの取得に失敗しました");
    return;
  }

  // ホワイトバランスを自動に設定
  s->set_whitebal(s, 1);  // 0 = Off, 1 = On

  // 上下方向の反転の有無
  s->set_vflip(s, 0);  // 0 = 正常, 1 = 上下反転

  // 左右方向の反転の有無
  //s->set_hmirror(s, 0);  // 0 = 正常, 1 = 左右反転

  // 他の設定例
  // s->set_brightness(s, 1);  // -2 to 2
  // s->set_contrast(s, 1);    // -2 to 2
  // s->set_saturation(s, 1);  // -2 to 2

  //16枚を空撮影、ホワイトバランス確保のため
  for (int i = 0; i < 16; i++) {
    // カメラから画像を取得
    Camera_fb = esp_camera_fb_get();
    if (!Camera_fb) {
      // SoftSerial.println("画像の取得に失敗しました");
      Serial.println("画像の取得に失敗しました");
      delay(1000);
      continue;
    }
    esp_camera_fb_return(Camera_fb);  // フレームバッファを解放
    delay(100);
  }
}

void loop() {

  // カメラから画像を取得
  Camera_fb = esp_camera_fb_get();
  if (!Camera_fb) {
    // SoftSerial.println("画像の取得に失敗しました");
    Serial.println("画像の取得に失敗しました");
    return;
  }

  uint16_t main = Camera_fb->buf[76800]*0xFF + Camera_fb->buf[76801];
  // main = 0b1111100000000000;
  Serial.print(" Min:");
  Serial.print(0);
  Serial.print(" Max1:");
  Serial.print(31);
  Serial.print(" Max2:");
  Serial.print(63);
  Serial.print(" R:");
  // Serial.print(((main & 0b0000011111100000) >> 5) / 2);
  Serial.print((main & 0b1111100000000000) >> 11);
  Serial.print(" G:");
  Serial.print(main >> 11);
  Serial.print(" B:");
  Serial.print(main & 0b0000000000000000);

  // SoftSerial.println(Camera_fb->len);
  // Serial.print(Camera_fb->len);
  // SoftSerial.println(Camera_fb->buf[1]);
  // Serial.print(" G:");
  // Serial.print(((Camera_fb->buf[10001] & 0x07) << 3) | (Camera_fb->buf[10000] >> 5));
  // SoftSerial.println(Camera_fb->buf[2]);
  // Serial.print(" B:");
  // Serial.println(Camera_fb->buf[10000] & 0x1F);
  // Serial.println((Camera_fb->buf[1] * 256) + Camera_fb->buf[0], HEX);
  
  esp_camera_fb_return(Camera_fb);

  // delay(250);

  Serial.println();
}