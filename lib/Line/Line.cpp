#include "Line.h"

MeassurementResult Line::result;
bool Line::correctionLeft = false;
bool Line::correctionRight = false;
bool Line::lastRed = false;
bool Line::lastBlue = false;
bool Line::lastGreen = false;

// vars for siren
unsigned long last = 0;
bool state = false;

void Line::loop() { 
  // get sensor values
  result = Sensors::getColorValues(PRINT_VALUES);

  // checkss if sensors disconnected
  if(result.color1.lux>DISCONNECTED || result.color2.lux>DISCONNECTED) {
    // turns off
    Motor::setSpeeds(0);
    LightManager::setBrakeLights(true);
    return;
  }

  // checks if left sensor is away from line
  if (result.color1.lux < MIN_LIGHT_LEVEL && !correctionRight) {
    // set motors to rotate
    motor2.setSpeed(0);
    motor1.setSpeed(-SPEED_ROTATE-SPEED_BACK_OFFSET);
    // short break so that both sensors have to start up
    vTaskDelay(1);
    motor2.setSpeed(COUNTER_ROTATE?SPEED_ROTATE:0);

    // set parms
    correctionLeft = true;
    LightManager::setBrakeLights(true);

  } else if(!correctionRight) {
    // set motors to drive straight
    motor1.setSpeed(SPEED_DRIVE);
    motor2.setSpeed(SPEED_DRIVE);

    // set parms
    correctionLeft = false;
    LightManager::setBrakeLights(false);
  }

  // checks if right sensor is away from line
  if(result.color2.lux < MIN_LIGHT_LEVEL && !correctionLeft) {
    // set motors to rotate
    motor1.setSpeed(0);
    motor2.setSpeed(-SPEED_ROTATE-SPEED_BACK_OFFSET);
    // short break so that both sensors have to start up
    vTaskDelay(1);
    motor1.setSpeed(COUNTER_ROTATE?SPEED_ROTATE:0);

    // set parms
    correctionRight = true;
    LightManager::setBrakeLights(true);

  } else if(!correctionLeft) {
    // set motors to drive straight
    motor2.setSpeed(SPEED_DRIVE);
    motor1.setSpeed(SPEED_DRIVE);

    // set parms
    correctionRight = false;
    LightManager::setBrakeLights(false);
  }

  // if no correction execute SafeMode-loop (since it would delay too much for the correction)
  if(!correctionLeft && !correctionRight) {
    SafeMode::loop();
  }


  // check colors

  // check blue (currently silver)
  if(result.color1.lux>LIGHT_LEVEL_BLUE && result.color2.lux>LIGHT_LEVEL_BLUE) {
    // if first reading on blue
    if(!lastBlue) {
      lastBlue = true;

      // set parms
      Speaker::startSignal2();
      Motor::halfTurn();
      Speaker::stopHupe();
    }

  // resets if not on blue
  } else if(result.color1.lux<LIGHT_LEVEL_NORMAL && result.color2.lux<LIGHT_LEVEL_NORMAL)
    lastBlue = false;


  //check green
  if((result.color1.lux>LIGHT_LEVEL_GREEN && result.color1.b<LIGHT_LEVEL_GREEN && result.color1.r<LIGHT_LEVEL_GREEN) || (result.color2.lux>LIGHT_LEVEL_GREEN && result.color2.b<LIGHT_LEVEL_GREEN && result.color2.r<LIGHT_LEVEL_GREEN)) {
    // if first reading on green or if siren needs to be updated
    if(!lastGreen || millis()-last > SIREN_INTERVAL) {
      lastGreen = true;

      // set parms
      state=!state;
      last = millis();
      ledcWriteTone(0, state?SIREN_LOW:SIREN_HIGH);
    }

  // resets if not on green
  } else if(result.color1.b>22&&result.color1.r>22&&result.color2.b>22&&result.color2.r>22) {
    lastGreen = false;
    state = false;
    Speaker::stopHupe();
  }

  // check red (currently not in use)
  if(result.color1.r>LIGHT_LEVEL_RED && result.color2.r>LIGHT_LEVEL_RED) {
    // if first reading on red
    if(!lastRed) {
      lastRed = true;

      // do stuff
    }

  // resets if not on red
  } else {
    lastRed = false;
  }
}

