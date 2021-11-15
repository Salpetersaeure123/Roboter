#include <Arduino.h>
long x; long y;
long c;
int z=5;
long d; long dEnd=1000; long c2;
void setup() {
  Serial.begin(115200);
  pinMode(16,OUTPUT);
}

void loop() {
  x=micros();
  y=micros();
  while(y<x+1000000) {
    digitalWrite(16,HIGH);
    delayMicroseconds(z);
    c2=0;
    for(d=1; d<=dEnd; d=d+1) {
       c2=c2+1;
    }
    digitalWrite(16,LOW);
    delayMicroseconds(z);
    
    y=micros();
    c=c+1;
  }
  Serial.println(c);
  c=0;
  delay(1000);
  
}