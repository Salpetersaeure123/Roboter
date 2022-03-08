#include "Line.h"

#define SPEED_ROTATE    0.22
#define SPEED_DRIVE     0.4

#define MODE false

MeassurementResult Line::result;
bool Line::correctionLeft = false;
bool Line::correctionRight = false;
bool Line::lastRed = false;
bool Line::lastBlue = false;
bool Line::lastGreen = false;
unsigned long last = 0;
bool state = false;

void Line::loop() { 
  result = Sensors::getColorValues(true);
  if(result.color1.lux>70 || result.color2.lux>70) {
    Motor::setSpeeds(0);
    LightManager::setBremsLight(true);
    return;
  }
  if (result.color1.lux < 22 && !correctionRight) {
    motor2.setSpeed(0);
    motor1.setSpeed(-SPEED_ROTATE-0.08);
    vTaskDelay(1);
    motor2.setSpeed(MODE?0:SPEED_ROTATE);
    correctionLeft = true;
    LightManager::setBremsLight(true);
  } else if(!correctionRight) {
    motor1.setSpeed(SPEED_DRIVE);
    motor2.setSpeed(SPEED_DRIVE);
    correctionLeft = false;
    LightManager::setBremsLight(false);
  }

  if(result.color2.lux < 22 && !correctionLeft) {
    motor1.setSpeed(0);
    motor2.setSpeed(-SPEED_ROTATE-0.08);
    vTaskDelay(1);
    motor1.setSpeed(MODE?0:SPEED_ROTATE);
    correctionRight = true;
    LightManager::setBremsLight(true);
  } else if(!correctionLeft) {
    motor2.setSpeed(SPEED_DRIVE);
    motor1.setSpeed(SPEED_DRIVE);
    correctionRight = false;
    LightManager::setBremsLight(false);
  }
  if(!correctionLeft && !correctionRight) {
    SafeMode::loop();
  }


  if(result.color1.lux>37&&result.color2.lux>37) {
    if(!lastBlue) {
      lastBlue = true;
      // Motor::setSpeeds(0);
      // unsigned long start = millis();
      // while(millis()-start < 700) {
      //   Speaker::signal();
      // }
      Speaker::startSignal2();
      Motor::halfTurn();
      Speaker::stopHupe();
    }
  } else if(result.color1.lux<26&&result.color2.lux<26)
    lastBlue = false;

  if((result.color1.lux>20&&result.color1.b<20&&result.color1.r<20)||(result.color2.lux>20&&result.color2.b<20&&result.color2.r<20)) {
    if(!lastGreen || millis()-last > 1000) {
      lastGreen = true;
      state=!state;
      last = millis();
      ledcWriteTone(0, state?440:587);
      // Speaker::startSignal2();
    }
  } else if(result.color1.b>22&&result.color1.r>22&&result.color2.b>22&&result.color2.r>22) {
    lastGreen = false;
    state = false;
    Speaker::stopHupe();
  }
  // if(result.color1.r>1&&result.color2.r>1) {
  //   if(!lastRed) {
  //     lastRed = true;
  //     Speaker::startSignal2();
  //     // Motor::setSpeeds(0);
  //     // unsigned long start = millis();
  //     // while(millis()-start < 700) {
  //     //   Speaker::signal();
  //     // }
  //     // Motor::setSpeeds(SPEED_DRIVE);
  //   }
  // } else {
  //   Speaker::stopHupe();
  //   lastRed = false;
  // }
}

