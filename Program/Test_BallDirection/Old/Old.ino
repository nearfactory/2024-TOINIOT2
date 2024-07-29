// #include<cmath>

// constexpr double PI = 3.14159265;

constexpr int BALL_NUM = 16;
const uint8_t BALL_PIN[BALL_NUM] = {
  A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15
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
  for(int i=0;i<BALL_NUM;i++) ball[i] = 1023 - analogRead(BALL_PIN[i]);



  // 全てのセンサの値
  short ball_x_all = 0;
  short ball_y_all = 0;
  double ball_dir_all = 0.0;

  for(int i=0;i<BALL_NUM;i++){
    double sensor_dir = (i-1)*360.0/16.0 + 180.0 - 67.5;
    // Serial.print(":nsensor:");
    // Serial.print(sensor_dir);
    ball_x_all += ball[i] * cos(sensor_dir/180.0*3.14);
    ball_y_all += ball[i] * sin(sensor_dir/180.0*3.14);
  }
  ball_dir_all = atan2(ball_y_all, ball_x_all) * 180.0 / 3.14 - 90.0;
  // Serial.print("\n\n\nx:");
  // // Serial.print(ball_x_all);
  // Serial.print("\ny:");
  // // Serial.print(ball_y_all);
  // Serial.print("\ndir:");
  // Serial.print(ball_dir_all);


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
    int j = (ball_big_index + BALL_NUM + i) % BALL_NUM;
    double sensor_dir = (j-1)*360.0/16.0 + 90.0 - 67.5;

    // Serial.print(" id");
    // Serial.print(i);
    // Serial.print(":");
    // Serial.print(j);

    // Serial.print("\nse");
    // Serial.print(i);
    // Serial.print(":");
    // Serial.print(sensor_dir);

    ball_x_2 += ball[j] * cos(sensor_dir/180.0*3.14);
    ball_y_2 += ball[j] * sin(sensor_dir/180.0*3.14);
  }
  ball_dir_2 = atan2(ball_y_2, ball_x_2) * (180.0 / PI);
  // Serial.print("\ndir:");
  // Serial.print(ball_dir_2);
  // Serial.print("\n");


  // 左右3つのセンサ
  short ball_x_3 = 0;
  short ball_y_3 = 0;
  double ball_dir_3 = 0;
  for(int i=-3;i<=3;i++){
    int j = (ball_big_index + BALL_NUM + i) % BALL_NUM;
    double sensor_dir = (j-1)*360.0/16.0 + 90.0 - 67.5;
    ball_x_3 += ball[j] * cos(sensor_dir/180.0*3.14);
    ball_y_3 += ball[j] * sin(sensor_dir/180.0*3.14);
  }
  ball_dir_3 = atan2(ball_y_3, ball_x_3) * (180.0 / PI);


  ball_dir_all = ball_dir_all < -180 ? 360 + ball_dir_all : ball_dir_all;
  Serial.print("\nmin:");
  Serial.print(-180);
  Serial.print("\nmax:");
  Serial.print(180);

  Serial.print("\nall:");
  Serial.print(ball_dir_all);
  Serial.print("\n2:");
  Serial.print(ball_dir_2);
  Serial.print("\n3:");
  Serial.print(ball_dir_3);
  Serial.print("\n");


  // Serial.printf("all:%lf, 2:%lf, 3:%lf\n", ball_dir_all, ball_dir_2, ball_dir_3);
  // for(auto b:ball){
  //   Serial.print(b);
  //   Serial.print(" ");
  // }
  // Serial.print("\ntime:");
  // Serial.print( millis()-begin_ms );
  // Serial.print("ms\n\n\n");
}
