#include "RemoteControl.h"
#include "Line.h"
#include "Ultrasonic.h"
#include "Speaker.h"


void loop_(void*);

void setup() {
  // start Serial
#if DEBUG
  Serial.begin(115200);
#endif

  // setup classes
  Speaker::setup();
  Sensors::init();
  LightManager::setup();
  RemoteControl::setup();

  // start loop task (has higher priority then default loop)
  xTaskCreate(loop_, "loop", 4*1024, NULL, 10, NULL);
}

void loop_(void*) {
  for(;;) {
    // get current mode and execute corresponding loop
    switch(RemoteControl::getMode()) {
      case LINE: Line::loop(); break;
      case ULTRASONIC: Ultrasonic::loop2(); vTaskDelay(100); break;
      case REMOTE: if(motor1.getDirection()==motor2.getDirection()&&(motor1.getSpeed()!=0)==(motor2.getSpeed()!=0)) SafeMode::loop(); vTaskDelay(100+max(motor1.getSpeed(), motor2.getSpeed())); break; // safeties for false triggers, if turning or driving too fast ultrasonic will trigger regardless of distance
      default: vTaskDelay(100); break;
    }
  }
}

void loop() {

}