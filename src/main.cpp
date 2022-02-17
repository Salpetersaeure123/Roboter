#include "Motor.h"
#include "Sensors.h"
#include "SR04.h"
#include "RemoteControl.h"
#include "Arduino.h"

#define normalSpeed1 0.50
#define normalSpeed2 0.45
#define speed1 0.45
#define speed2 0.40

#define off 0.00

int fourDistances [4] {

};
int direction;

SR04 ultrasonic = SR04(SONIC_ECHO, SONIC_TRIG);
long distance;

void hupe();

void setup() {
#if DEBUG
  Serial.begin(115200);
#endif
  ledcSetup(0, 20000, 8);
  ledcAttachPin(13, 0);

  Sensors::init();
  // ultrasonic setup siehe oben
  //Serial.println("ultrasonic setup done");
  // RemoteControl::setup();

  // motor1.setSpeed(normalSpeed1);
  // motor2.setSpeed(normalSpeed2);

  // hupe();
}

bool correctionleft = false;
bool correctionright = false;

void quarterTurn();
void halfTurn();
void fullTurn();
void lineLoop();
void ultrasonicLoop();
void ultrasonicLoop2();

void loop() {
  hupe();
  motor1.setSpeed(.7);
  motor2.setSpeed(.7);
  //ultrasonicLoop2();
  // return;
  switch(RemoteControl::getMode()) {
    case LINE: lineLoop(); break;
    case ULTRASONIC: ultrasonicLoop(); break;
    // case REMOTE: RemoteControl::loop(); break;
    /*case REMOTE:  distance = ultrasonic.Distance();
    if(distance <= 10) {
      RemoteControl::correction = true;
      halfTurn();
      RemoteControl::correction = false;
    }
    break;*/
    default: break;
  }
}

void fullTurn() {
  //does a 360° turn
  motor1.setSpeed(-speed1);
  motor2.setSpeed(speed2);
  delay(2200);
}

void halfTurn() {
  //does a 180° turn
  motor1.setSpeed(-speed1);
  motor2.setSpeed(speed2);
  delay(1100);
  motor1.setSpeed(0);
  motor2.setSpeed(0);
}

void quarterTurn() {
  //does a 90° turn
  motor1.setSpeed(-speed1);
  motor2.setSpeed(speed2);
  delay(550);
}

void ultrasonicLoop2() {
  distance = ultrasonic.Distance();
  if(distance < 15) {
    distance = ultrasonic.Distance();
    if(distance < 15) {
      Serial.println("ausweichen");
      long highest_distance = 0;
      unsigned long startTime = millis();
      motor1.setSpeed(0.42);
      motor2.setSpeed(-0.40);
      //höchste Werte finden
      while(millis()-startTime<4000) {
        distance = ultrasonic.Distance();
        if(distance > highest_distance) {
          highest_distance = distance;
          motor1.setSpeed(0);
          motor2.setSpeed(0);
          delay(50);
          motor1.setSpeed(0.42);
          motor2.setSpeed(-0.40);
        }
        if(distance>200) {
          motor1.setSpeed(normalSpeed1);
          motor2.setSpeed(normalSpeed2);
          return;
        }
      }
      Serial.println("höchster wert wiederfinden");
      long new_highest_distance = 0;
      while(true) {
        startTime = millis();
        while(millis()-startTime<4000) {
          distance = ultrasonic.Distance();
          if(distance>=highest_distance) {
            motor1.setSpeed(normalSpeed1);
            motor2.setSpeed(normalSpeed2);
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
            motor1.setSpeed(normalSpeed1);
            motor2.setSpeed(normalSpeed2);
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
    motor1.setSpeed(normalSpeed1);
    motor2.setSpeed(normalSpeed2);
  }
}

void ultrasonicLoop() {
  distance = ultrasonic.Distance();
  if(distance < 50) {
    distance = ultrasonic.Distance();
    if(distance < 50) {
      for(int c=0; c<=3; c=c+1) {
        motor1.setSpeed(off);
        motor2.setSpeed(off);
        delay(250);
        distance = ultrasonic.Distance();
        fourDistances [c] = distance;
        quarterTurn();
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
        quarterTurn();
      }
      motor1.setSpeed(normalSpeed1);
      motor2.setSpeed(normalSpeed2);
    }
  }
  else {
    motor1.setSpeed(normalSpeed1);
    motor2.setSpeed(normalSpeed2);
  }
}

void lineLoop() {
  Serial.println("line");
  MeassurementResult result;
  result = Sensors::getColorValues(false);
  // Sensors::getLidarValues(true);

  if (result.color1.lux < 1 && !correctionright) {
    motor1.setSpeed(-speed1);
    motor2.setSpeed(speed2);
    //digitalWrite(LED_R, 1);
    correctionleft = true;
  }
  else {
    //digitalWrite(LED_R, 0);
    motor1.setSpeed(normalSpeed1);
    correctionleft = false;
  }

  if (result.color2.lux < 1 && !correctionleft) {
    motor2.setSpeed(-speed2);
    motor1.setSpeed(speed1);
    //digitalWrite(LED_B, 1);
    correctionright = true;
  }
  else {
    motor2.setSpeed(normalSpeed2);
    //digitalWrite(LED_B, 0);
    correctionright = false;
  }
  if(!correctionleft && !correctionright) {
    distance = ultrasonic.Distance();
    if(distance <= 10) {
      halfTurn();
    }
  }
  
  // Serial.print(motor1.getDirection());
  // Serial.print("\t");
  // Serial.print(motor1.getSpeed());
  // Serial.print("\t2:");
  // Serial.print(motor2.getDirection());
  // Serial.print("\t");
  // Serial.println(motor2.getSpeed());
  // result.difference.lux+= 17;
  // if(abs(result.difference.lux)>27&&!(result.color1.lux>100&&result.color2.lux>100)) {
  //   if(correction)
  //     return;
  //   (result.difference.lux>0?motor1:motor2).setSpeed(0.55);
  //   correction = true;
  //   digitalWrite(result.difference.lux>0?LED_R:LED_B, 1);
  //   digitalWrite(result.difference.lux>0?LED_B:LED_R, 0);
  //   digitalWrite(LED_G, 0);
  // } else {
  //   if(result.color1.lux<100)
  //     return;
  //   motor1.setSpeed(normalSpeed1);
  //   motor2.setSpeed(normalSPeed2);
  //   correction = false;
  //   digitalWrite(LED_R, 0);
  //   digitalWrite(LED_B, 0);
  //   digitalWrite(LED_G, 1);
  // }
}

void hupe() {
    ledcWrite(0, 255);
    delayMicroseconds(1250);
    ledcWrite(0, 0);
    delayMicroseconds(1250);
    //Serial.println(i);
  /*ledcWriteNote(0, NOTE_G, 4);
  delay(1000);
  ledcWrite(0, 0);
  delay(3000);*/
}