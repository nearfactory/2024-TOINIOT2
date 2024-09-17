// 新機体想定
// UI基板 1番ボタンで出力値を加算 / 2番ボタンで出力値を減算
// シリアルプロッタに出力

constexpr uint8_t BTN1_PIN = 36; // 出力値加算
constexpr uint8_t BTN2_PIN = 37; // 出力値減算

constexpr int SAMPLE_NUM = 5;   // 移動平均のサンプル数
int motor = 0;                  // プログラム側から設定する用の出力値
int motor_sum = 0;              // 出力値の合計
int motor_raw = 0;              // モーターに適用する用の出力値
int motor_queue[SAMPLE_NUM]{};  // 出力値のキュー
int motor_queue_index = 0;      // 出力値のキューのインデックス

void setup() {
  Serial.begin(9600);
  Serial.println("TOINIOT2 Moving Average Test 2024-09-17");

  pinMode(BTN1_PIN, INPUT);
  pinMode(BTN2_PIN, INPUT);
}

void loop() {
  // 出力値を計算
  motor += digitalRead(BTN1_PIN)*50;
  motor -= digitalRead(BTN2_PIN)*50;

  // 範囲制限
  motor = ( motor > 255 ? 255 : motor ) < 0 ? 0 : motor;

  // キューに追加
  motor_queue[motor_queue_index] = motor;
  motor_queue_index = (motor_queue_index+1) % SAMPLE_NUM;

  // 出力値を計算
  for(auto m:motor_queue) motor_sum += m;
  motor_raw = motor_sum / SAMPLE_NUM;

  // 出力
  Serial.printf("motor:%d motor_raw:%d motor_queue_index:%d\n", motor, motor_raw, motor_queue_index);
}