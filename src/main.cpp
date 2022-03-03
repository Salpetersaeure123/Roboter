#include "RemoteControl.h"
#include "Line.h"
#include "Ultrasonic.h"
#include "Speaker.h"

long us1; long us2;

void setup() {
#if DEBUG
  Serial.begin(115200);
#endif
  Speaker::setup();
  Sensors::init();
  LightManager::setup();
  RemoteControl::setup();
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
    case ULTRASONIC: Ultrasonic::loop2();
    case REMOTE: SafeMode::loop(); break;
    default: break;
  }
  // Sensors::getUltrasonicValues(true);
  // Serial.println(millis()-us1);
  // us1 = millis();
  /*for(double d =2.00; d>= 0.20; d=d-0.10) {
    us1=micros();
    while(micros() < us1 + 200000) {
      Speaker::hupe(d);
    }
  }*/
  // us1 = micros();
  // while(micros() < us1 + 1500000) {
  //     Speaker::hupe(0.7);
  // }
  // delay(2000);
  // Speaker::signal();
}
