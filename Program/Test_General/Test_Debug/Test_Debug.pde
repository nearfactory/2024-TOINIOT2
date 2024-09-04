void setup(){
  size(1000,1000);
  windowTitle("あ");
  strokeWeight(2);
  stroke(0,64,230);
}

int val[] = new int[11];
void draw(){
  background(255);
  for(int i=0;i<10;i++){
    val[i] = val[i+1];
  }
  val[9]  = random(0,1000);

  for(int i=0;i<11;i++){
    line(i*100, val[i], (i+1)*100, val[i+1]);
  }
}