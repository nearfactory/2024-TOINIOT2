#include "Sub.hpp"

void Sub::begin(){
  Serial2.begin(115200);
  return;
}

void Sub::read(){
  while(Serial2.available()){
    char c = Serial2.read();

    switch(count){
    case 0:
      ball01k = c;
      queue[queue_id][0] = ball01k;
      count++;
      break;
    case 1:
      ball02k = c;
      queue[queue_id][1] = ball02k;
      count++;
      break;
    default:
      count = 0;
      break;
    }
    queue_id = (queue_id+1)%QUEUE_SIZE;
  }

  int sum1 = 0;
  int sum2 = 0;
  for(int i=0;i<QUEUE_SIZE;i++){
    sum1 += queue[i][0];
    sum2 += queue[i][1];
  }
  ball01k = sum1 / QUEUE_SIZE;
  ball02k = sum2 / QUEUE_SIZE;

  if(is_hold){
    if(ball01k > 130 || ball02k > 130){
      is_hold = false;
    }
  }else{
    if(ball01k < 75 || ball02k < 75){
      is_hold = true;
    }
  }

  return;
}