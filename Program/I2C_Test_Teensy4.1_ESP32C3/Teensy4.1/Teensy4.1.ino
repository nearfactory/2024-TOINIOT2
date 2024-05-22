// Wire Slave Sender
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Sends data as an I2C/TWI slave device
// Refer to the "Wire Master Reader" example for use with this

// Created 29 March 2006

// This example code is in the public domain.

#include <string>
#include <Wire.h>

#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <utility/imumaths.h>

int led = LED_BUILTIN;

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);

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

  /*
  //printEvent(&orientationData);
  //printEvent(&angVelocityData);
  //printEvent(&linearAccelData);
  //printEvent(&magnetometerData);
  //printEvent(&accelerometerData);
  //printEvent(&gravityData);

  int8_t boardTemp = bno.getTemp();
  Serial.println();
  Wire.write(F("temperature: "));
  Serial.println(boardTemp);

  uint8_t system, gyro, accel, mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);
  Serial.println();
  Wire.write("Calibration: Sys=");
  Wire.write(system);
  Wire.write(" Gyro=");
  Wire.write(gyro);
  Wire.write(" Accel=");
  Wire.write(accel);
  Wire.write(" Mag=");
  Serial.println(mag);

  Serial.println("--");
  */

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
                   

  //could add VECTOR_ACCELEROMETER, VECTOR_MAGNETOMETER,VECTOR_GRAVITY...
  sensors_event_t orientationData , angVelocityData , linearAccelData, magnetometerData, accelerometerData, gravityData;
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
  Serial.println("val");
  printEvent(&gravityData);
  digitalWrite(led, LOW);
}

void printEvent(sensors_event_t* event) {
  std::string str{};
  double x = -1000000, y = -1000000 , z = -1000000; //dumb values, easy to spot problem
  if (event->type == SENSOR_TYPE_ACCELEROMETER) {
    //Wire.write("Accl:");
    str+="Accl:";
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
  }
  else if (event->type == SENSOR_TYPE_ORIENTATION) {
    // Wire.write("Orient:");
    str+="Orient:";
    x = event->orientation.x;
    y = event->orientation.y;
    z = event->orientation.z;
  }
  else if (event->type == SENSOR_TYPE_MAGNETIC_FIELD) {
    // Wire.write("Mag:");
    str+="Mag:";
    x = event->magnetic.x;
    y = event->magnetic.y;
    z = event->magnetic.z;
  }
  else if (event->type == SENSOR_TYPE_GYROSCOPE) {
    // Wire.write("Gyro:");
    str+="Gyro";
    x = event->gyro.x;
    y = event->gyro.y;
    z = event->gyro.z;
  }
  else if (event->type == SENSOR_TYPE_ROTATION_VECTOR) {
    // Wire.write("Rot:");
    str+="Rot";
    x = event->gyro.x;
    y = event->gyro.y;
    z = event->gyro.z;
  }
  else if (event->type == SENSOR_TYPE_LINEAR_ACCELERATION) {
    // Wire.write("Linear:");
    str+="Linear:";
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
  }
  else if (event->type == SENSOR_TYPE_GRAVITY) {
    // Wire.write("Gravity:");
    str+="Gravity:";
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
  }
  else {
    // Wire.write("Unk:");
    str+="Unk:";
  }

  /*
  Wire.write("\tx= ");
  Wire.write(std::to_string(x).c_str());
  Wire.write(" |\ty= ");
  Wire.write(std::to_string(y).c_str());
  Wire.write(" |\tz= ");
  Wire.print(std::to_string(z).c_str());
  */
  str+="\tx="+std::to_string(x)+"|\ty="+std::to_string(y)+"|\tz="+std::to_string(z)+"\n";
  Wire.write(str.c_str());
}