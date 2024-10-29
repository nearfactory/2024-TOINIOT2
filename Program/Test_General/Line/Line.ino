/*
*/
#include<stdlib.h>

void setup() {
  Serial.begin(9600);
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(2,INPUT_PULLUP);
  pinMode(3,OUTPUT);
}

char buf[10]{};
int threshold = 0;
int i = 0;

void loop() {
  for(auto& c:buf) c='\0'; 
  while(Serial.available()){
    buf[i] = Serial.read();
    i++;
  }
  if(i!=0) threshold = atoi(buf);
  i=0;
  analogWrite(3, threshold);

  Serial.print("min:0  max:1023");
  Serial.print("  threshold:");
  Serial.print(threshold);
  Serial.print("  comp:");
  Serial.print(digitalRead(2));
  Serial.print("  line1:");
  Serial.println(analogRead(A0)/4);
  // Serial.print("  line2:");
  // Serial.println(analogRead(A1));
  delay(50);
}

// 13 

/*
void setup(){
  Serial.begin(9600);
  pinMode(3,OUTPUT);
  for(int i=7;i<=52;i++) pinMode(i, INPUT_PULLUP);
}

void loop(){
  analogWrite(3,128);
  for(int i=7;i<=52;i++){
    if(!digitalRead(i)){
      Serial.print(i);
      Serial.print(" ");
    }
  }
  Serial.println();
}
*/

/*
void setup(){
  Serial.begin(9600);
  pinMode(53, INPUT_PULLUP);
}

void loop(){
  Serial.println(digitalRead(53));
}
*/