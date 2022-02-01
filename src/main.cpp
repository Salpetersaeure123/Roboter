#include "Motor.h"
#include "Sensors.h"

Motor motor1(18, 19);
Motor motor2(17, 16);

void setup() {
#if DEBUG
  Serial.begin(115200);
#endif

  Sensors::init();
  
  // motor1.setSpeed(.8);
  // motor2.setSpeed(.8);
}

void loop() {
  if(DEBUG_COLOR)
    Sensors::printColorValues();
  if(DEBUG_LIDAR)
    Sensors::printLidarValues();
  delay(1000);
}