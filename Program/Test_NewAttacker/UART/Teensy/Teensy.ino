// #include<Wire.h>

// #include<Adafruit_SSD1306.h>
// #include<Adafruit_GFX.h>

// Adafruit_SSD1306 display(128,64,&Wire2,-1);

void setup(){
  // uart
  Serial2.begin(115200);

  // test
  // pinMode(0,INPUT);
  // pinMode(1,OUTPUT);

  Serial.begin(9600);

  // Wire2.begin();
  // while(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)){
  //   ;
  // }

  // display.clearDisplay();
  // display.setTextSize(1);
  // display.setTextColor(SSD1306_WHITE);
  // display.display();
}

void loop(){
  // display.clearDisplay();
  // display.setCursor(0,8);
  // display.println("uart test");
  // uart
  while(Serial2.available()){
    char c = Serial2.read();
    //Serial.println(Serial1.read());
    Serial.print(c);
    // display.print(c);
  }
  /*
  if(Serial.available()){
    Serial1.print(Serial.read());
  }
  */
  Serial2.println("Teensy4.1");

  // test
  // static bool val=false;
  // static int ms=0;
  // if(millis()-ms>250){
  //   val=!val;
  //   ms=millis();
  // }
  // Serial.println(digitalRead(0));
  // digitalWrite(1,val);
  
  // display.display();
  delay(50);
}