void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
  Serial4.begin(9600);
}

void loop() {
  // Serial1
  if(Serial1.available()){
    Serial.print("Serial1:");
    while(Serial1.available()){
      Serial.print((char)Serial1.read());
    }
    Serial1.println("Teensy-Serial1");
  }
  
  // Serial2
  if(Serial2.available()){
    Serial.print("Serial2:");
    while(Serial2.available()){
      Serial.print((char)Serial2.read());
    }
    Serial2.println("Teensy-Serial2");
  }

  // Serial3
  if(Serial3.available()){
    Serial.print("Serial3:");
    while(Serial3.available()){
      Serial.print((char)Serial3.read());
    }
    Serial3.println("Teensy-Serial3");
  }

  // Serial4
  if(Serial4.available()){
    Serial.print("Serial4:");
    while(Serial4.available()){
      Serial.print((char)Serial4.read());
    }
    Serial4.println("Teensy-Serial4");
  }

  delay(250);
}
