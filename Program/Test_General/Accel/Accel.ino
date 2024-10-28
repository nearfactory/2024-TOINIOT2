#include <Adafruit_BNO055.h>
#include <Wire.h>

#define smaller(x,y) ((x)<(y) ? (x) : (y))
#define bigger(x,y) ((x)>(y) ? (x) : (y))

// constexpr float delay = 50.0;
// int delay = 50;

Adafruit_BNO055 bno(55, 0x28, &Wire);

void setup() {
  while(!bno.begin());

  Serial.begin(9600);
  Serial.println("BNO Test!");
}

typedef struct{
  float x,y,z;
}Data_t;

void loop() {
  static int count=0;
  sensors_event_t raw;
  bno.getEvent(&raw, Adafruit_BNO055::VECTOR_LINEARACCEL);

  static float x_acc = 0;
  static float x_acc_p = 0;
        x_acc = raw.orientation.x;
        x_acc = abs(x_acc)>0.05 ? x_acc : 0;

  static float x_ver = 0;
  static float x_ver_p = 0;
        float x_ver_b = (x_acc_p + x_acc) * 0.025;
        // x_ver += abs(x_ver_b)>0.1 ? x_ver_b : 0;
        x_ver = x_ver_b;

  static float x_pos = 0;
  static float x_pos_p =0;
        x_pos += (x_ver_p + x_ver) * 0.025;
        // x_pos += (x_ver_p + x_ver) * delay / 2000;

  static float x_nazo = 0;
        // x_nazo += abs(x_pos)>0.01 ? x_pos : 0;
        x_nazo += (x_pos_p + x_pos) * 0.025;

  // x_acc = bigger(x_acc, -0.1);
  // x_acc = smaller(x_acc, 0.1);x
  x_acc = 0;
  // x_nazo = 0;c
  Serial.printf("acc:%f, ver:%f, pos:%f, nazo:%f\n", x_acc, x_ver, x_pos, x_nazo);

  x_acc_p = x_acc;
  x_ver_p = x_ver;
  x_pos_p = x_pos;

  count-=-1;
  delay(50);
}
