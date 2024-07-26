#include<cmath>

constexpr int BALL_NUM = 16;
const uint8_t BALL_PIN[BALL_NUM] = {
  14, 15, 16, 17, 18, 19, 20, 21, 22, 23,  26, 27, 38, 39, 40, 41
  // A0, A1, A2, A3, A4, A5, A6, A7, A8, A9,  A12, A13, A14, A15, A16, A17
};

void setup() {
  Serial.begin(9600);
  Serial.println("Ball Direction Test Program 2024-07-27");

  for(auto p:BALL_PIN) pinMode(p,INPUT);
}

void loop() {
  unsigned int begin_ms = millis();
  unsigned short ball[BALL_NUM]{0};
  for(int i=0;i<BALL_NUM;i++) ball[i] = analogRead(BALL_PIN[i]);



  // 全てのセンサの値
  short ball_x_all = 0;
  short ball_y_all = 0;
  double ball_dir_all = 0.0;

  for(int i=0;i<BALL_NUM;i++){
    double sensor_dir = (i-1)*360.0/16.0 + 90.0;
    ball_x_all += ball[i] * cos(sensor_dir);
    ball_y_all += ball[i] * sin(sensor_dir);
  }
  ball_dir_all = atan2(ball_y_all, ball_x_all) * (3.14 / 180.0);



  // ボールセンサ値の最大値・そのセンサ番号を算出
  short ball_big = 0;
  short ball_big_index = 0;
  for(int i=0;i<BALL_NUM;i++){
    if(ball_big < ball[i]){
      ball_big = ball[i];
      ball_big_index = i;
    }
  }



  // 左右2つのセンサ
  short ball_x_2 = 0;
  short ball_y_2 = 0;
  double ball_dir_2 = 0;
  for(int i=-2;i<=2;i++){
    int j = (ball_big_index + i) % BALL_NUM;
    double sensor_dir = (j-1)*360.0/16.0 + 90.0;
    ball_x_2 += ball[j] * cos(sensor_dir);
    ball_y_2 += ball[j] * sin(sensor_dir);
  }
  ball_dir_2 = atan2(ball_y_2, ball_x_2) * (3.14 / 180.0);
  


  // 左右2つのセンサ
  short ball_x_3 = 0;
  short ball_y_3 = 0;
  double ball_dir_3 = 0;
  for(int i=-3;i<=3;i++){
    int j = (ball_big_index + i) % BALL_NUM;
    double sensor_dir = (j-1)*360.0/16.0 + 90.0;
    ball_x_3 += ball[j] * cos(sensor_dir);
    ball_y_3 += ball[j] * sin(sensor_dir);
  }
  ball_dir_3 = atan2(ball_y_3, ball_x_3) * (3.14 / 180.0);



  Serial.printf("all:%lf, 2:%lf, 3:%lf\n", ball_dir_all, ball_dir_2, ball_dir_3);
  for(auto b:ball) Serial.printf("%d ", b);
  Serial.printf("\ntime:%d(ms)\n", millis()-begin_ms );
}
