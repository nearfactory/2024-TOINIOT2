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

#include <algorithm>
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

#define LUT_SIZE 256

SoftwareSerial SoftSerial;

camera_fb_t* Camera_fb;

struct Obj{
  uint8_t obj_id = 0;       // オブジェクト識別用のID
  int16_t x1 = 321; // バウンディングボックスの左座標
  int16_t x2 = -1;  // 右
  int16_t y1 = 241; // 上
  int16_t y2 = -1;  // 下
  int32_t area = 0;
};  

uint8_t* buf  = NULL;
uint8_t* buf2 = NULL;
uint8_t* id_lut  = NULL;  // LUT(仮のIDと本当のIDを対応)
uint8_t* obj_lut  = NULL; // LUT(本当のIDとオブジェクトIDを対応)
Obj* obj = NULL;

void printBuf(uint8_t* buf_ptr){
  Serial.println("\n\n\n----------------");

  for(int y=0;y<QVGA_Y/3;y++){
    for(int x=BUF_SPACE;x<QVGA_X+BUF_SPACE;x++){
      if(buf_ptr[x+(y*3+BUF_SPACE)*BUF_X]==0){
        Serial.print(" ");
      }else if(buf_ptr[x+(y*3+BUF_SPACE)*BUF_X]==255){
        Serial.print("/");
      }else{
        Serial.printf("%c", (buf_ptr[x+(y*3+BUF_SPACE)*BUF_X]+25)%26+65);
        // Serial.print(buf_ptr[x+(y*3+BUF_SPACE)*BUF_X]);
      }
    }
    Serial.println();
    if(y==39){
      for(int i=0;i<QVGA_X;i++) Serial.print("-");
      Serial.println();
    }
  }

  Serial.println("----------------\n\n\n");
}

void printBufFull(uint8_t* buf_ptr){
  Serial.println("\n\n\n----------------");

  // int aaa = 590;
  // while(true){
    
  // }

  for(int y=BUF_SPACE;y<QVGA_Y+BUF_SPACE;y++){
    for(int x=BUF_SPACE;x<QVGA_X+BUF_SPACE;x++){
      if(buf_ptr[x+y*BUF_X]==0){
        Serial.print(" ");
      // }else{
      //   Serial.print(buf_ptr[x+y*3*BUF_X]);
      // }
      }else if(buf_ptr[x+y*BUF_X]==255){
        Serial.print("/");
      }else{
        Serial.printf("%c", (id_lut[buf_ptr[x+y*BUF_X]]+1)%26+65);
        // Serial.printf("%c", obj_lut[buf_ptr[x+y*BUF_X]]%26+65);
        // Serial.print(buf_ptr[x+y*BUF_X]);
        // Serial.printf("%c", (buf_ptr[x+y*BUF_X]+25)%26+65);
      }
      // Serial.print(buf[x+y*3*BUF_X]);
    }
    Serial.println();
  }

  Serial.println("----------------\n\n\n");
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
  buf     = reinterpret_cast<uint8_t*>(malloc(sizeof(int8_t)*BUF_X*BUF_Y)); // しきい値を超えたピクセルのバッファ
  buf2    = reinterpret_cast<uint8_t*>(malloc(sizeof(int8_t)*BUF_X*BUF_Y)); // しきい値を超えたピクセルのバッファ
  id_lut  = reinterpret_cast<uint8_t*>(malloc(sizeof(int8_t)*LUT_SIZE));         // 仮のID → 本当のID
  obj_lut = reinterpret_cast<uint8_t*>(malloc(sizeof(int8_t)*LUT_SIZE));         // 本当のID → オブジェクトID
  obj     = reinterpret_cast<Obj*>(malloc(sizeof(Obj)*LUT_SIZE));               // オブジェクト
  
  if(buf == NULL || buf2 == NULL || id_lut == NULL || obj_lut == NULL || obj == NULL){
    Serial.println("buf err!");
    return;
  } 

  // 初期化をサボるな！
  for(int i=0;i<BUF_X*BUF_Y;i++){
    buf[i] = 0;
    buf2[i] = 0;
  }
  for(int i=0;i<LUT_SIZE;i++){
    id_lut[i] = i;
    obj_lut[i] = i;
  }
  // printBuf(buf);
  // printBuf(buf2);
  // Serial.printf("%08d", buf);
  // Serial.println("\n\n");

  delay(100);

}

uint32_t begin_ms = millis();
uint32_t processing_begin = millis();
void loop() {
  // PCからの入力を待機
  while(!Serial.available()){}
  while( Serial.available()) Serial.read();



  // カメラから画像を取得
  // begin_ms = millis();
  processing_begin = millis();
  Camera_fb = esp_camera_fb_get();
  if (!Camera_fb) {
    // SoftSerial.println("画像の取得に失敗しました");
    Serial.println("画像の取得に失敗しました");
    return;
  }
  Serial.printf("\n\nGet Image\t\t\t:%d\n", millis()-begin_ms);

  // 青
  // HSV : 214 73 86
  int h_min_b = 184;
  int h_max_b = 220;
  int s_min_b = 64;
  int v_min_b = 128;

  // 黄色
  // HSV : 49 62 76
  int h_min_y = 40;
  int h_max_y = 75;
  int s_min_y = 50;
  int v_min_y = 180;
  h_min_y = 0;
  h_max_y = 0;
  s_min_y = 100;
  v_min_y  = 255;

  // しきい値を超えたピクセルのバッファを作る
  begin_ms = millis();
  for(int y=0;y<QVGA_Y;y++){
    for(int x=0;x<QVGA_X;x++){
      // for(int i=0;i<Camera_fb->len/2;i++){
      // uint16_t pixel = (Camera_fb->buf[i*2]<<8) + (Camera_fb->buf[i*2+1]);
      uint16_t pixel = (Camera_fb->buf[(x+y*QVGA_X)*2]<<8) + (Camera_fb->buf[(x+y*QVGA_X)*2+1]);
      uint8_t r = ((pixel & 0b1111100000000000) >> 11) << 3;
      uint8_t g = ((pixel & 0b0000011111100000) >> 6)  << 3;
      uint8_t b = ((pixel & 0b0000000000011111))       << 3;

      // RGB → HSV で色を識別
      // H : 0 - 360
      // S : 0 - 255
      // V : 0 - 255
      // アルゴリズム : https://qiita.com/jajagaccxhi/items/e64ff4b347eceafce5d3
      // しきい値     : https://python.joho.info/opencv/opencv-color-detection/
      int h=0, s=0, v=0;

      int Max = std::max({ r, g, b });
      int Min = std::min({ r, g, b });
      if (Min == Max) h = 0;
      else if (Min == b) h = 60 * (g - r)  / (Max - Min) + 60;
      else if (Min == r) h = 60 * (b - g)  / (Max - Min) + 180;
      else if (Min == g) h = 60 * (r - b)  / (Max - Min) + 300;
      v = Max;
      s = Max - Min;

      // 二値化
      if( (h_min_b < h && h < h_max_b  &&  s_min_b < s  &&  v_min_b < v)  ||  (h_min_y < h && h < h_max_y  &&  s_min_y < s  &&  v_min_y < v) ){
        // buf[i+BUF_SPACE + BUF_SPACE*2*i/QVGA_X] = 1;
        buf[x+BUF_SPACE + (y+BUF_SPACE)*BUF_X] = 1;
      }else{
        // buf[i+BUF_SPACE + BUF_SPACE*2*i/QVGA_X] = 0;
        buf[x+BUF_SPACE + (y+BUF_SPACE)*BUF_X] = 0;
      }
    }
  }

  // printBuf(buf);
  Serial.printf("Binarization\t\t\t:%d\n", millis()-begin_ms);

  // buf2のクリーン
  // begin_ms = millis();
  for(int i=0;i<BUF_X*BUF_Y;i++) buf2[i] = 0;
  // Serial.printf("Clean Buf2\t\t\t:%d\n", millis()-begin_ms);

  // ノイズ除去 (膨張・収縮)
  // https://www.frontier.maxell.co.jp/blog/posts/22.html
  // begin_ms = millis();
  int REPEAT = 3;
  uint8_t* read_buf = buf;
  uint8_t* write_buf = buf2;



  // 膨張 → 収縮
  // 膨張
  begin_ms = millis();
  for(int i=0;i<REPEAT;i++){
    for(int y=BUF_SPACE;y<QVGA_Y+BUF_SPACE;y++){
      for(int x=BUF_SPACE;x<QVGA_X+BUF_SPACE;x++){
        int i=x+BUF_X*y;
        // ピクセルの周囲8方向に1があるか調べる
        /*
        if(read_buf[i-BUF_X-1]||read_buf[i-BUF_X]||read_buf[i-BUF_X+1] || read_buf[i-1]||read_buf[i]||read_buf[i+1] || read_buf[i+BUF_X-1]||read_buf[i+BUF_X]||read_buf[i+BUF_X+1]) // 8近傍
        // if(read_buf[i-BUF_X] || read_buf[i-1]||read_buf[i+1] || read_buf[i+BUF_X]) // 4近傍
        // if(read_buf[i])
        {
          write_buf[i] = 1;
        }else{
          write_buf[i] = 0;
        }
        */
        write_buf[i] = read_buf[i-BUF_X-1] | read_buf[i-BUF_X] | read_buf[i-BUF_X+1]   |   read_buf[i-1] | read_buf[i] | read_buf[i+1]   |   read_buf[i+BUF_X-1] | read_buf[i+BUF_X] | read_buf[i+BUF_X+1];
      }
    }
    // バッファの入れ替え
    uint8_t* buf_temp = read_buf;
    read_buf = write_buf; // 二値化画像
    write_buf = buf_temp; // IDの記録画像
  }
  // printBuf(read_buf);
  Serial.printf("Denoise1\t\t\t:%d\n", millis()-begin_ms);

  // 収縮
  begin_ms = millis();
  for(int i=0;i<REPEAT;i++){
    for(int y=BUF_SPACE;y<QVGA_Y+BUF_SPACE;y++){
      for(int x=BUF_SPACE;x<QVGA_X+BUF_SPACE;x++){
        int i=x+BUF_X*y;
        /*
        // ピクセルの周囲8方向に0があるか調べる
        if(!(read_buf[i-BUF_X-1]&&read_buf[i-BUF_X]&&read_buf[i-BUF_X+1] && read_buf[i-1]&&read_buf[i]&&read_buf[i+1] && read_buf[i+BUF_X-1]&&read_buf[i+BUF_X]&&read_buf[i+BUF_X+1]))  // 8近傍
        // if(read_buf[i-BUF_X-1]==0||read_buf[i-BUF_X]==0||read_buf[i-BUF_X+1]==0 || read_buf[i-1]==0||read_buf[i]==0||read_buf[i+1]==0 || read_buf[i+BUF_X-1]==0||read_buf[i+BUF_X]==0||read_buf[i+BUF_X+1]==0)  // 8近傍
        // if(read_buf[i-BUF_X]==1 && read_buf[i-1]==1&&read_buf[i+1]==1 && read_buf[i+BUF_X]==1) // 4近傍
        // if(!read_buf[i])
        {
          write_buf[i] = 0;
        }else{
          write_buf[i] = 1;
        }
        */
        write_buf[i] = read_buf[i-BUF_X-1] & read_buf[i-BUF_X] & read_buf[i-BUF_X+1]   &   read_buf[i-1] & read_buf[i] & read_buf[i+1]   &   read_buf[i+BUF_X-1] & read_buf[i+BUF_X] & read_buf[i+BUF_X+1];
      }
    }
    // バッファの入れ替え
    uint8_t* buf_temp = read_buf;
    read_buf = write_buf; // 二値化画像
    write_buf = buf_temp; // IDの記録画像
  }
  // printBuf(read_buf);
  Serial.printf("Denoise2\t\t\t:%d\n", millis()-begin_ms);
  

  // 収縮 → 膨張
  // 収縮
  begin_ms = millis();
  for(int i=0;i<REPEAT;i++){
    for(int y=BUF_SPACE;y<QVGA_Y+BUF_SPACE;y++){
      for(int x=BUF_SPACE;x<QVGA_X+BUF_SPACE;x++){
        int i=x+BUF_X*y;
        // ピクセルの周囲8方向に0があるか調べる
        if(!(read_buf[i-BUF_X-1]&&read_buf[i-BUF_X]&&read_buf[i-BUF_X+1] && read_buf[i-1]&&read_buf[i]&&read_buf[i+1] && read_buf[i+BUF_X-1]&&read_buf[i+BUF_X]&&read_buf[i+BUF_X+1]))  // 8近傍
        // if(!(read_buf[i-BUF_X] && read_buf[i-1]&&read_buf[i+1] && read_buf[i+BUF_X])) // 4近傍
        // if(!read_buf[i])
        {
          write_buf[i] = 0;
        }else{
          write_buf[i] = 1;
        }
      }
    }
    uint8_t* buf_temp = read_buf;
    read_buf = write_buf; // 二値化画像
    write_buf = buf_temp; // IDの記録画像
  }
  // printBuf(read_buf);
  Serial.printf("Denoise3\t\t\t:%d\n", millis()-begin_ms);

  // 膨張
  begin_ms = millis();
  for(int i=0;i<REPEAT;i++){
    for(int y=BUF_SPACE;y<QVGA_Y+BUF_SPACE;y++){
      for(int x=BUF_SPACE;x<QVGA_X+BUF_SPACE;x++){
        int i=x+BUF_X*y;
        /*
        // ピクセルの周囲8方向に1があるか調べる
        if(read_buf[i-BUF_X-1]||read_buf[i-BUF_X]||read_buf[i-BUF_X+1] || read_buf[i-1]||read_buf[i]||read_buf[i+1] || read_buf[i+BUF_X-1]||read_buf[i+BUF_X]||read_buf[i+BUF_X+1]) // 8近傍
        // if(read_buf[i-BUF_X] || read_buf[i-1]||read_buf[i+1] || read_buf[i+BUF_X]) // 4近傍
        // if(read_buf[i])
        {
          write_buf[i] = 255;
        }else{
          write_buf[i] = 0;
        }
        */
        write_buf[i] = read_buf[i-BUF_X-1] | read_buf[i-BUF_X] | read_buf[i-BUF_X+1]   |   read_buf[i-1] | read_buf[i] | read_buf[i+1]   |   read_buf[i+BUF_X-1] | read_buf[i+BUF_X] | read_buf[i+BUF_X+1];
      }
    }
    uint8_t* buf_temp = read_buf;
    read_buf = write_buf; // 二値化画像
    write_buf = buf_temp; // IDの記録画像
  }
  // printBuf(read_buf);
  Serial.printf("Denoise4\t\t\t:%d\n", millis()-begin_ms);
  // printBufFull(write_buf);

  // printBuf(buf);



  // 物体認識
  // https://imagingsolution.blog.fc2.com/blog-entry-193.html
  // https://qiita.com/IcchI1/items/102fec377f63c6c03d79
  // 物体認識用にポインタの入れ替え
  // uint8_t* buf_temp = read_buf;
  // read_buf = write_buf; // 二値化画像
  // write_buf = buf_temp; // IDの記録画像

  // バッファの初期化
  // begin_ms = millis();
  for(int i=0;i<BUF_X*BUF_Y;i++) write_buf[i] = 0;
  // Serial.printf("Initialize write_buf\t\t:%d\n", millis()-begin_ms);

  // ラベリング
  begin_ms = millis();
  uint8_t next_id = 1;
  for(int i=0;i<256;i++){
    id_lut[i] = i;
  }

  for(int y=BUF_SPACE;y<QVGA_Y+BUF_SPACE;y++){
    if(next_id>120) break;
    for(int x=BUF_SPACE;x<QVGA_X+BUF_SPACE;x++){
      int i = x+BUF_X*y;
      if(read_buf[i]){
        // 上3・左1から最小のIDを探す
        int minimum   = 255;
        int secondary = 255;  

        int num1 = write_buf[i-BUF_X-1];
        int num2 = write_buf[i-BUF_X];
        int num3 = write_buf[i-BUF_X+1];
        int num4 = write_buf[i-1];
        if(num1<minimum && num1!=0) {secondary = minimum;  minimum = num1;}
        if(num2<minimum && num2!=0) {secondary = minimum;  minimum = num2;}
        if(num3<minimum && num3!=0) {secondary = minimum;  minimum = num3;}
        if(num4<minimum && num4!=0) {secondary = minimum;  minimum = num4;}

        // 周囲に最小のIDがない
        if(minimum == 255){
          write_buf[i] = next_id;
          id_lut[minimum] = minimum;
          next_id++;
          // Serial.println("a");
        }else{
          write_buf[i] = minimum;
          // LUTを更新
          id_lut[secondary] = minimum;
        }
      }
    
    }
  }
  Serial.printf("Labeling\t\t\t:%d\n", millis()-begin_ms);
  // printBuf(write_buf);

  // LUTをもとにラベルを更新
  begin_ms = millis();
  for(int y=BUF_SPACE;y<QVGA_Y+BUF_SPACE;y++){
    for(int x=BUF_SPACE;x<QVGA_X+BUF_SPACE;x++){
      int i = x+BUF_X*y;
      while(id_lut[write_buf[i]] != write_buf[i]){
        write_buf[i] = id_lut[write_buf[i]];
      }
    }
  }
  // printBuf(write_buf);
  Serial.printf("Update Label\t\t\t:%d\n", millis()-begin_ms);
  
  // LUTの中身を表示
  // Serial.println();
  // for(int i=0;i<LUT_SIZE;i++) printf("%d ", id_lut[i]);

  // Serial.printf("\n%d\n", next_id);
  // printBufFull(write_buf);



  // ラスタスキャン → ぶち当たったピクセルのLUT上のIDをオブジェクトのIDに変換し、obj内の各座標変数と位置を比較する
  // LUT2, obj配列
  // ！物体認識メイン！
  begin_ms = millis();
  for(int i=0;i<LUT_SIZE;i++){
    obj_lut[i] = 0;
  } 

  int8_t next_obj_id = 0;
  // 余計な部分を圧縮して、本当のID→オブジェクトIDのテーブルを作る
  for(int i=1;i<next_id;i++){
    if(id_lut[i]==i){
      obj_lut[i] = next_obj_id;
      next_obj_id++;
    }
  }
  // Serial.printf("Compression Lut\t\t\t:%d\n", millis()-begin_ms);

  // for(int i=0;i<LUT_SIZE;i++){
  //   Serial.printf("%d ", id_lut[i]);
  // }
  // Serial.println();
  // for(int i=0;i<LUT_SIZE;i++){
  //   Serial.printf("%d ", obj_lut[i]);
  // }
  // Serial.println();

  // Serial.printf("ObjectNum:%d\n\n", next_obj_id);

  // オブジェクト配列を初期化
  begin_ms = millis();
  for(int i=0;i<next_obj_id;i++){
    obj[i].obj_id = i;
    obj[i].x1 = 321;
    obj[i].y1 = 241;
    obj[i].x2 = -1;
    obj[i].y2 = -1;
  }
   
  // 物体の座標を求める
  for(int y=0;y<QVGA_Y;y++){
    for(int x=0;x<QVGA_X;x++){
      int i = (x+BUF_SPACE)+BUF_X*(y+BUF_SPACE);
      if(write_buf[i] != 0){
        if(x < obj[obj_lut[write_buf[i]]].x1) obj[obj_lut[write_buf[i]]].x1 = x; // 左
        if(x > obj[obj_lut[write_buf[i]]].x2) obj[obj_lut[write_buf[i]]].x2 = x; // 右

        if(y < obj[obj_lut[write_buf[i]]].y1) obj[obj_lut[write_buf[i]]].y1 = y; // 上
        if(y > obj[obj_lut[write_buf[i]]].y2) obj[obj_lut[write_buf[i]]].y2 = y; // 下
      }
    }
  }
  Serial.printf("Caliculate Coordinate\t\t:%d\n", millis()-begin_ms);

  // 面積を求める
  begin_ms = millis();
  int max_area = 0;
  int max_area_id = 0;
  for(int i=0;i<next_obj_id;i++){
    obj[i].area = (obj[i].x2 - obj[i].x1) * (obj[i].y2 - obj[i].y1);
    if(obj[i].area > max_area){
      max_area = obj[i].area;
      max_area_id = i;
    }
  }
  Serial.printf("Caliculate Area\t\t\t:%d\n", millis()-begin_ms);
  Serial.printf("Total\t\t\t\t:%d\n", millis()-processing_begin);

  // オブジェクト配列全部吐く
  // for(int i=0;i<next_obj_id;i++){
  //   Serial.printf("id:%02d    x1:%03d  y1:%03d   x2:%03d  y2:%03d   area:%d\n", obj[i].obj_id, obj[i].x1, obj[i].y1, obj[i].x2, obj[i].y2, obj[i].area);
  // }

  // 最大の面積のものを出力
  // Serial.printf("x1:%03d  y1:%03d   x2:%03d  y2:%03d   id:%02d    area:%d    processing:%d(ms)\n", obj[max_area_id].x1, obj[max_area_id].y1, obj[max_area_id].x2, obj[max_area_id].y2, obj[max_area_id].obj_id, (obj[max_area_id].x2 - obj[max_area_id].x1) * (obj[max_area_id].y2 - obj[max_area_id].y1), millis() - begin_ms);

  esp_camera_fb_return(Camera_fb);
  begin_ms = millis();
}