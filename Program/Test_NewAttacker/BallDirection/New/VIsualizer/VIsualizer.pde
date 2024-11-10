import processing.serial.*;

Serial Port;
int[] ball = new int[16];

void setup(){
  size(800,800);  
  frameRate(60);
  
  printArray(Serial.list());
  
  int port_id = 2;
  String PortStr = Serial.list()[port_id];
  Port = new Serial(this, PortStr, 115200);
}

int val = 0;
float ball_dir = 0;
float ball_dir_correct = 0;

void draw(){
  String str = " ";
  while(Port.available() != 0 && str.charAt(str.length()-1)!='\n'){
    str += Port.readChar();
  }  
  str.trim();
  // print(str);

  float ball_x = 0, ball_y = 0;
  if(str.length()>80){
    for(int i=0;i<16;i++){
      ball[i] = 1023 - Integer.parseInt(str.substring(i*5+1,i*5+5));
      // print(ball[i] + " ");
      
      float sensor_dir = radians(i*360.0/16.0);
      ball_x+=ball[i]*cos(sensor_dir);
      ball_y+=ball[i]*sin(sensor_dir);
    }
    ball_dir = degrees(atan2(-ball_y, ball_x));
    // println();
  }
  
  int small = 1023;
  int small_id = 0;
  int big = 0;
  int big_id = 0;
  for(int i=0;i<16;i++){
    if(ball[i]<small) {
      small = ball[i];
      small_id = i;
    }
    if(ball[i]>big){
      big = ball[i];
      big_id = i;
    }
  }
  float big_dir = -big_id*360/16.0;
  if(big_dir<-180) big_dir = 360 + big_dir;
  
  float small_dir = -small_id*360/16.0;
  if(small_dir<-180) small_dir = 360+small_dir;
  
  ball_dir_correct = (ball_dir + big_dir) / 2 ;
  
  float ball_dir_correct2 = ball_dir_correct;
  
  
  small_dir += 180;
  if(small_dir>180) small_dir = small_dir -= 360;
  
  if(abs(ball_dir-big_dir) > 5){
    
    // ball_dir_correct2 = (ball_dir_correct+small_dir)/2;
  }
  
  println(small_dir);


  background(32);
  translate(width/2,height/2);
  rotate(radians(-90));
  
  
  noFill();
  stroke(200);
  strokeWeight(2);
  ellipse(0, 0, 1023/1.25, 1023/1.25);
  strokeWeight(1);
  ellipse(0, 0, 512/1.25, 512/1.25);
  
  
  for(int i=0;i<16;i++){
    // ball
    // fill(230,16,16);
    fill(180, 180, 180);
    noStroke();
    rect(0, -20, ball[i]/2.5, 40);
    
    // grid
    noFill();
    stroke(200);
    line(0,0,cos(radians(360/32))*1023/2.5,sin(radians(360/32))*1023/2.5);
    
    rotate(radians(-360.0/16.0));
  }
  
  
  noFill();
  strokeWeight(4);
  // big
  stroke(230,230,32);
  line(0,0,cos(radians(-small_id*360/16.0))*150,sin(radians(-small_id*360/16.0))*150);
  // small
  stroke(230,32,230);
  line(0,0,cos(radians(-big_id*360/16.0))*150,sin(radians(-big_id*360/16.0))*150);
  // raw
  stroke(32,32,230);
  line(0,0,cos(radians(ball_dir))*400, sin(radians(ball_dir))*400);
  // correct
  stroke(32,230,32);
  line(0,0,cos(radians(ball_dir_correct))*300, sin(radians(ball_dir_correct))*300);
  // correct2
  stroke(230,32,32);
  line(0,0,cos(radians(ball_dir_correct2))*400, sin(radians(ball_dir_correct2))*400);
  
  /*
  for(int i=0;i<16;i++){
    String buf = str.substring(0,3);
    buf = "100";
    ball[i] = Integer.parseInt(buf);
    ball[i] = 100;
    print(ball[i] + " ");
  }
  */
}
