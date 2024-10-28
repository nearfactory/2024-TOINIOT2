#include <string>
#include <Wire.h>

#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <utility/imumaths.h>

int led = LED_BUILTIN;

std::string str{};
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);
sensors_event_t orientationData , angVelocityData , linearAccelData, magnetometerData, accelerometerData, gravityData;

void setup()
{
  Serial.println(9600);

  while(!Serial) delay(10);
  Serial.println("Orientation sensor test\n");

  if(!bno.begin()){
    Serial.println("BNO cannnot begin");
    while(1) ;
  }

  delay(1000);

  pinMode(led, OUTPUT);
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
}

void loop()
{
  str="";

  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
  bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);
  bno.getEvent(&magnetometerData, Adafruit_BNO055::VECTOR_MAGNETOMETER);
  bno.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
  bno.getEvent(&gravityData, Adafruit_BNO055::VECTOR_GRAVITY);

  printEvent(&orientationData);
  printEvent(&angVelocityData);
  printEvent(&linearAccelData);
  printEvent(&magnetometerData);
  printEvent(&accelerometerData);
  printEvent(&gravityData);

  delay(100);
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent()
{
  //static int count=0;
  //count++;
  digitalWrite(led, HIGH);  // briefly flash the LED
  //Wire.write(std::to_string(count).c_str());    // respond with message of 6 bytes
                                                // as expected by master

  // sensors_event_t magnetometerData;

  //Wire.write("value");

  //Serial.println("val");
  digitalWrite(led, LOW);
  Wire.write(str.c_str());
  Serial.println(str.c_str());
}

void printEvent(sensors_event_t* event) {
  //static int val=0;
  //val++;
  //str+=std::to_string(val);
  double x = -1000000, y = -1000000 , z = -1000000; //dumb values, easy to spot problem
  if (event->type == SENSOR_TYPE_ACCELEROMETER) {
    str+="Accl:";
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
  }
  else if (event->type == SENSOR_TYPE_ORIENTATION) {
    str+="Orient:";
    x = event->orientation.x;
    y = event->orientation.y;
    z = event->orientation.z;
  }
  else if (event->type == SENSOR_TYPE_MAGNETIC_FIELD) {
    str+="Mag:";
    x = event->magnetic.x;
    y = event->magnetic.y;
    z = event->magnetic.z;
  }
  else if (event->type == SENSOR_TYPE_GYROSCOPE) {
    str+="Gyro";
    x = event->gyro.x;
    y = event->gyro.y;
    z = event->gyro.z;
  }
  else if (event->type == SENSOR_TYPE_ROTATION_VECTOR) {
    str+="Rot";
    x = event->gyro.x;
    y = event->gyro.y;
    z = event->gyro.z;
  }
  else if (event->type == SENSOR_TYPE_LINEAR_ACCELERATION) {
    str+="Linear:";
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
  }
  else if (event->type == SENSOR_TYPE_GRAVITY) {
    str+="Gravity:";
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
  }
  else {
    str+="Unk:";
  }

  str+="\tx="+std::to_string(x)+"|\ty="+std::to_string(y)+"|\tz="+std::to_string(z)+"\n";
  //Wire.write(str.c_str());
}