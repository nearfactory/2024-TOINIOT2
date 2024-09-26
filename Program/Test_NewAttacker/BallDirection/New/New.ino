#include<cmath>

constexpr int BALL_NUM = 16;
const uint8_t BALL_PIN[BALL_NUM] = {
  14, 15, 16, 17, 18, 19, 20, 21, 22, 23,  26, 27, 38, 39, 40, 41
};

void setup() {
  Serial.begin(115200);
  Serial.println("Ball Direction Test Program 2024-07-27");

  for(auto p:BALL_PIN) pinMode(p,INPUT);
}

// ボールを一番見ているセンサーの左右ひとつづつの数値の偏りを比較する

void loop() {
  unsigned int begin_ms = millis();
  unsigned short ball[BALL_NUM]{0};
  // センサ値取得
  for(int i=0;i<BALL_NUM;i++) ball[i] = analogRead(BALL_PIN[i]);

  // 最小値/ID 算出
  int min=1023;
  int min_id= 0;
  for(int i=0;i<BALL_NUM;i++){
    if(min>ball[i]){
      min = ball[i];
      min_id = i;
    }
  }

  int left_id = (min_id-1+BALL_NUM)%BALL_NUM;
  int right_id = (min_id+1)%BALL_NUM;

  int center_val = ball[min_id];
  int left_val   = ball[left_id];
  int right_val  = ball[right_id];

  int center     = center_val;
  int left_diff  = left_val  - center;
  int right_diff = right_val - center;

  double ball_x = 0;
  double ball_y = 0;
  for(int i=left_id;i<=right_id;i++){
    double sensor_dir = radians(i*360.0/16.0);
    ball_x += ball[i]*cos(sensor_dir);
    ball_y += ball[i]*sin(sensor_dir);
  }
  double ball_dir = degrees(atan2(ball_y,ball_x));
  double diff_max = left_diff>right_diff ? left_diff : right_diff;
  
  ball_dir = min_id*360.0/16.0;
  // ball_dir = 0;

  for(int i=0;i<BALL_NUM;i++){
    double sensor_dir = radians(i*360.0/16.0);
    ball_x+=ball[i]*cos(sensor_dir);
    ball_y+=ball[i]*sin(sensor_dir);
  }
  ball_dir = degrees(atan2(ball_y,ball_x));

  // ball_dir += 11.25*(1-left_diff/diff_max);
  // ball_dir -= 11.25*(1-right_diff/diff_max);

  Serial.println("\n");
  Serial.printf("%d\t\t%lf\t\t", min_id, ball_dir);
  // Serial.printf("%d\t%d\t%d\t\t",left_id,min_id,right_id);
  // Serial.printf("%d\t%d\t%d\t\t",left_val,center_val,right_val);
  Serial.printf("%d\t%d\t%d\t\t",left_diff,center,right_diff);

  delay(50);
}
