#include <Arduino.h>
#include <ArduinoJson.h>
#include <esp_camera.h>

#include "camera_pins.h"

// ボーレート
#define BAUDRATE 115200

camera_fb_t* Camera_fb;

void setup() {
  Serial.begin(BAUDRATE);

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
  config.pixel_format = PIXFORMAT_JPEG;
  //config.frame_size = FRAMESIZE_QVGA;  // フレームサイズをQVGAに設定
  //config.frame_size = FRAMESIZE_VGA;
  config.frame_size = FRAMESIZE_XGA;
  config.jpeg_quality = 12;  // JPEG品質を調整
  config.fb_count = 2;
  config.grab_mode = CAMERA_GRAB_LATEST;

  while (true) {
    if (esp_camera_init(&config) != ESP_OK) {
      Serial.println("カメラの初期化に失敗しました");
    } else {
      Serial.println("カメラが初期化されました");
      break;
    }
    delay(1000);
  }

  // カメラセンサーの取得
  sensor_t* s = esp_camera_sensor_get();
  if (s == NULL) {
    Serial.println("センサーの取得に失敗しました");
    return;
  }

  // ホワイトバランスを自動に設定
  s->set_whitebal(s, 1);  // 0 = Off, 1 = On

  s->set_vflip(s, 0);  // 0 = 正常, 1 = 上下反転
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
      Serial.println("画像の取得に失敗しました");
      delay(1000);
      continue;
    }
    esp_camera_fb_return(Camera_fb);  // フレームバッファを解放
    delay(100);
  }

  //WriteSlackChnnel("書き込みのテストです。\r\n");
}

void loop() {

  // カメラから画像を取得
  Camera_fb = esp_camera_fb_get();
  if (!Camera_fb) {
    Serial.println("画像の取得に失敗しました");
    return;
  }

  // JpegSlackChnnelUpLoad("sample.jpg", Camera_fb->buf, Camera_fb->len, "M5 Stack CamS3 Unit の画像です", "タイトル");
  esp_camera_fb_return(Camera_fb);

  //60秒待つ
  delay(60000);
}