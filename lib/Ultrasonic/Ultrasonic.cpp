#include "Ultrasonic.h"

int Ultrasonic::fourDistances[4] = {};
int Ultrasonic::direction = 0;
long Ultrasonic::distance = 0;

// does quarter turns
void Ultrasonic::loop() {
  // check distance
  distance = Sensors::getUltrasonicValues();
  if(distance < MIN_DISTANCE) {
    // ensure proper readings
    distance = Sensors::getUltrasonicValues();
    if(distance < MIN_DISTANCE) {
      // check all 4 directions
      for(int c=0; c<=3; c=c+1) {
        // stand still
        motor1.setSpeed(SPEED_OFF);
        motor2.setSpeed(SPEED_OFF);
        // wait a little bit for proper readings
        delay(250);
        // get ditance
        distance = Sensors::getUltrasonicValues();
        // store distance
        fourDistances [c] = distance;
        // move a quarter
        Motor::quarterTurn();

        // returns if mode has changed
        if(RemoteControl::getMode() != ULTRASONIC)
          return;
      }

      // searches for direction with most space
      direction = 0;
      for(int i = 1; i < 4; i++) {
        // sets direction as result if greater than before
        if(fourDistances[i]>fourDistances[direction])
          direction = i;
      }

      // turn to selected direction
      for(int c=0; c < direction; c=c+1) {
        Motor::quarterTurn();
      }

      // drive
      motor1.setSpeed(SPEED_DRIVE);
      motor2.setSpeed(SPEED_DRIVE);
    }
  } else {
    // drive
    motor1.setSpeed(SPEED_DRIVE);
    motor2.setSpeed(SPEED_DRIVE);
  }
}

// meassures continuesly
void Ultrasonic::loop2() {
  // check distance
  distance = Sensors::getUltrasonicValues();
  if(distance < MIN_DISTANCE) {
    // ensure proper readings
    distance = Sensors::getUltrasonicValues();
    if(distance < MIN_DISTANCE) {
      // starts searching for exit
      Serial.println("ausweichen");
      // set some vars
      long highest_distance = 0;
      unsigned long startTime = millis();
      // start rotating
      motor1.setSpeed(SPEED_ROTATE);
      motor2.setSpeed(-SPEED_ROTATE);
      
      // turns as long that even on hard ground one turn is done
      while(millis()-startTime<SEARCH_DURATION) {
        // get distance
        distance = Sensors::getUltrasonicValues();
        // stores distance if greatest yet
        if(distance > highest_distance) {
          highest_distance = distance;
          // do a short pause
          motor1.setSpeed(0);
          motor2.setSpeed(0);
          delay(50);
          motor1.setSpeed(SPEED_ROTATE);
          motor2.setSpeed(-SPEED_ROTATE);
        }
        // if distance great enough, drive imediately
        if(distance>LEAVE_DISTANCE) {
          // drive
          motor1.setSpeed(SPEED_DRIVE);
          motor2.setSpeed(SPEED_DRIVE);
          return;
        }
      }

      // search highest value again
      Serial.println("höchsten wert wiederfinden");
      long new_highest_distance = 0;
      while(true) {
        startTime = millis();
        while(millis()-startTime<SEARCH_DURATION) {
          // get distance
          distance = Sensors::getUltrasonicValues();
          // if distance is highest distance then drive
          if(distance>=highest_distance) {
            // drive
            motor1.setSpeed(SPEED_DRIVE);
            motor2.setSpeed(SPEED_DRIVE);
            Serial.println("weg gefunden");
            return;
          }
          // if distance is highest yet in new sample, store it
          if(distance > new_highest_distance) {
            new_highest_distance = distance;
            // short break
            motor1.setSpeed(0);
            motor2.setSpeed(0);
            delay(50);
            // continue rotating
            motor1.setSpeed(SPEED_ROTATE);
            motor2.setSpeed(-SPEED_ROTATE);
          }
          // if distance great enough, drive imediately
          if(distance>100) {
            motor1.setSpeed(SPEED_DRIVE);
            motor2.setSpeed(SPEED_DRIVE);
            return;
          }
        }
        // if highest distance not found again, set new highest distance as new goal
        highest_distance = new_highest_distance;
        new_highest_distance = 0;
        // start again searching for new highest distance
      }
    }
  // if distance is great enough drive fast
  } else if(distance > 100) {
    motor1.setSpeed(SPEED_FAST);
    motor2.setSpeed(SPEED_FAST);
  // else drive normally
  } else {
    motor1.setSpeed(SPEED_DRIVE);
    motor2.setSpeed(SPEED_DRIVE);
  }
}
