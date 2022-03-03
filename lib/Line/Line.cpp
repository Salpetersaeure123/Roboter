#include "Line.h"

#define SPEED_ROTATE    0.22
#define SPEED_DRIVE     0.3

MeassurementResult Line::result;
bool Line::correctionLeft = false;
bool Line::correctionRight = false;

void Line::loop() {
  result = Sensors::getColorValues(false);

  if (result.color1.lux < 1 && !correctionRight) {
    motor2.setSpeed(0);
    motor1.setSpeed(-SPEED_ROTATE-0.08);
    vTaskDelay(1);
    motor2.setSpeed(SPEED_ROTATE);
    correctionLeft = true;
    LightManager::setBremsLight(true);
  } else if(!correctionRight) {
    motor1.setSpeed(SPEED_DRIVE);
    correctionLeft = false;
    LightManager::setBremsLight(false);
  }

  if (result.color2.lux < 1 && !correctionLeft) {
    motor1.setSpeed(0);
    motor2.setSpeed(-SPEED_ROTATE-0.08);
    vTaskDelay(1);
    motor1.setSpeed(SPEED_ROTATE);
    correctionRight = true;
    LightManager::setBremsLight(true);
  } else if(!correctionLeft) {
    motor2.setSpeed(SPEED_DRIVE);
    correctionRight = false;
    LightManager::setBremsLight(false);
  }
  if(!correctionLeft && !correctionRight) {
    SafeMode::loop();
  }
}

