#include "Line.h"

#define SPEED_ROTATE    0.5
#define SPEED_DRIVE     0.8

MeassurementResult Line::result;
bool Line::correctionLeft = false;
bool Line::correctionRight = false;

void Line::loop() {
  result = Sensors::getColorValues(false);

  if (result.color1.lux < 1 && !correctionRight) {
    motor1.setSpeed(-SPEED_ROTATE);
    motor2.setSpeed(SPEED_ROTATE);
    correctionLeft = true;
  }
  else {
    motor1.setSpeed(SPEED_DRIVE);
    correctionLeft = false;
  }

  if (result.color2.lux < 1 && !correctionLeft) {
    motor2.setSpeed(-SPEED_ROTATE);
    motor1.setSpeed(SPEED_ROTATE);
    correctionRight = true;
  }
  else {
    motor2.setSpeed(SPEED_DRIVE);
    correctionRight = false;
  }
  if(!correctionLeft && !correctionRight) {
    if(Sensors::getUltrasonicValues() <= 10) {
      Motor::halfTurn();
    }
  }
}

