#include "RemoteControl.h"
#include "Line.h"
#include "Ultrasonic.h"
#include "Piezo.h"

void setup() {
#if DEBUG
  Serial.begin(115200);
#endif
  Piezo::setup();
  Sensors::init();
  RemoteControl::setup();
  pinMode(BACK_LIGHT_L, OUTPUT);
  pinMode(BACK_LIGHT_R, OUTPUT);
  digitalWrite(BACK_LIGHT_L, HIGH);
  digitalWrite(BACK_LIGHT_R, HIGH);
}

void loop() {
  // if(millis()%4000>2000) {
  // Motor::setSpeeds(0.);
  // analogWrite(BACK_LIGHT_L, 100);
  // analogWrite(BACK_LIGHT_R, 100);
  // // } else {
  // delay(1000);
  // for(int i = 60; i<= 255; i++) {
  //   Motor::setSpeeds(i);
  //   delay(10);
  // }
  // delay(1000);
  // analogWrite(BACK_LIGHT_L, 255);
  // analogWrite(BACK_LIGHT_R, 255);
  // Motor::setSpeeds(1.);
  // return;
  switch(RemoteControl::getMode()) {
    case LINE: Line::loop(); break;
    case ULTRASONIC: Ultrasonic::loop(); break;
    case REMOTE: break;
    default: break;
  }
}
