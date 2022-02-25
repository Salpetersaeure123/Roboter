#include "Ultrasonic.h"

#define SPEED_OFF       0
#define SPEED_DRIVE     0.8
#define SPEED_ROTATE    0.5

int Ultrasonic::fourDistances[4] = {};
int Ultrasonic::direction = 0;
long Ultrasonic::distance = 0;

void Ultrasonic::loop() {
  distance = Sensors::getUltrasonicValues();
  if(distance < 50) {
    distance = Sensors::getUltrasonicValues();
    if(distance < 50) {
      for(int c=0; c<=3; c=c+1) {
        motor1.setSpeed(SPEED_OFF);
        motor2.setSpeed(SPEED_OFF);
        delay(250);
        distance = Sensors::getUltrasonicValues();
        fourDistances [c] = distance;
        Motor::quarterTurn();
        if(RemoteControl::getMode() != ULTRASONIC)
          return;
      }
      direction = 0;
      for(int i = 1; i < 4; i++) {
        if(fourDistances[i]>fourDistances[direction])
          direction = i;
      }

      /*if(fourDistances[0] > fourDistances[1]) {
        if(fourDistances[0] > fourDistances[2]) {
          if(fourDistances[0] > fourDistances[3]) {
            direction = 0;
          }
        }
      }
      if(fourDistances[1] > fourDistances[0]) {
        if(fourDistances[1] > fourDistances[2]) {
          if(fourDistances[1] > fourDistances[3]) {
            direction = 1;
          }
        }
      }
      if(fourDistances[2] > fourDistances[0]) {
        if(fourDistances[2] > fourDistances[1]) {
          if(fourDistances[2] > fourDistances[3]) {
            direction = 2;
          }
        }
      }
      if(fourDistances[3] > fourDistances[0]) {
        if(fourDistances[3] > fourDistances[1]) {
          if(fourDistances[3] > fourDistances[2]) {
            direction = 3;
          }
        }
      }*/

      for(int c=0; c < direction; c=c+1) {
        Motor::quarterTurn();
      }
      motor1.setSpeed(SPEED_DRIVE);
      motor2.setSpeed(SPEED_DRIVE);
    }
  }
  else {
    motor1.setSpeed(SPEED_DRIVE);
    motor2.setSpeed(SPEED_DRIVE);
  }
}

void Ultrasonic::loop2() {
  distance = Sensors::getUltrasonicValues();
  if(distance < 15) {
    distance = Sensors::getUltrasonicValues();
    if(distance < 15) {
      Serial.println("ausweichen");
      long highest_distance = 0;
      unsigned long startTime = millis();
      motor1.setSpeed(SPEED_ROTATE);
      motor2.setSpeed(-SPEED_ROTATE);
      //höchste Werte finden
      while(millis()-startTime<4000) {
        distance = Sensors::getUltrasonicValues();
        if(distance > highest_distance) {
          highest_distance = distance;
          motor1.setSpeed(0);
          motor2.setSpeed(0);
          delay(50);
          motor1.setSpeed(SPEED_ROTATE);
          motor2.setSpeed(-SPEED_ROTATE);
        }
        if(distance>200) {
          motor1.setSpeed(SPEED_DRIVE);
          motor2.setSpeed(SPEED_DRIVE);
          return;
        }
      }
      Serial.println("höchsten wert wiederfinden");
      long new_highest_distance = 0;
      while(true) {
        startTime = millis();
        while(millis()-startTime<4000) {
          distance = Sensors::getUltrasonicValues();
          if(distance>=highest_distance) {
            motor1.setSpeed(SPEED_DRIVE);
            motor2.setSpeed(SPEED_DRIVE);
            Serial.println("weg gefunden");
            return;
          }
          if(distance > new_highest_distance) {
            new_highest_distance = distance;
            motor1.setSpeed(0);
            motor2.setSpeed(0);
            delay(50);
            motor1.setSpeed(0.42);
            motor2.setSpeed(-0.40);
          }
          if(distance>200) {
            motor1.setSpeed(SPEED_DRIVE);
            motor2.setSpeed(SPEED_DRIVE);
            return;
          }
        }
        highest_distance = new_highest_distance;
        new_highest_distance = 0;
      }
    }
  } else if(distance > 300 && distance < 500) {
    motor1.setSpeed(0.7);
    motor2.setSpeed(0.7);
  } else if(distance > 500) {
    motor1.setSpeed(1.);
    motor2.setSpeed(1.);
  } else {
    motor1.setSpeed(SPEED_DRIVE);
    motor2.setSpeed(SPEED_DRIVE);
  }
}
