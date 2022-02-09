#include "Motor.h"
#include "Sensors.h"
#include "SR04.h"
#include "RemoteControl.h"

Motor motor1(18, 19); // = left
Motor motor2(17, 16); // = right

#define LED_R 23
#define LED_G 22
#define LED_B 21

enum Mode {
  LINE,
  ULTRASONIC,
  REMOTE
};

Mode mode = LINE;

int fourDistances [4] {

};
int direction;

SR04 ultrasonic = SR04(SONIC_ECHO, SONIC_TRIG);
long distance;

void setup() {
#if DEBUG
  Serial.begin(115200);
#endif

  Sensors::init();
  // ultrasonic setup siehe oben
  Serial.println("ultrasonic setup done");
  RemoteControl::setup();

  motor1.setSpeed(0.45);
  motor2.setSpeed(0.40);

  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
}

bool correctionleft = false;
bool correctionright = false;

void quarterTurn();
void halfTurn();
void fullTurn();
void lineLoop();
void ultrasonicLoop();

void loop() {
  switch(mode) {
    case LINE: lineLoop(); break;
    case ULTRASONIC: ultrasonicLoop(); break;
    case REMOTE: RemoteControl::loop(); break;
  }
}

void fullTurn() {
  //does a 360° turn
  motor1.setSpeed(-0.42);
  motor2.setSpeed(0.33);
  delay(2500);
  motor1.setSpeed(0.0);
  motor2.setSpeed(0.0);
}

void halfTurn() {
  //does a 180° turn
  motor1.setSpeed(-0.42);
  motor2.setSpeed(0.33);
  delay(1250);
}

void quarterTurn() {
  //does a 90° turn
  motor1.setSpeed(-0.42);
  motor2.setSpeed(0.33);
  delay(625);
}

void ultrasonicLoop() {
  distance = ultrasonic.Distance();
  if(distance < 150) {
    for(int c=0; c<=3; c=c+1) {
      distance = ultrasonic.Distance();
      fourDistances [c] = distance;
      quarterTurn();
    }

    if(fourDistances[0] > fourDistances[1]) {
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
    }

    for(int c=0; c < direction; c=c+1) {
      quarterTurn();
    }
    motor1.setSpeed(0.45);
    motor2.setSpeed(0.40);
  }
}

void lineLoop() {
  MeassurementResult result;
  result = Sensors::getColorValues(false);
  // Sensors::getLidarValues(true);

  if (result.color1.lux < 1 && !correctionright) {
    motor1.setSpeed(-0.40);
    motor2.setSpeed(0.35);
    //digitalWrite(LED_R, 1);
    correctionleft = true;
  }
  else {
    //digitalWrite(LED_R, 0);
    motor1.setSpeed(0.45);
    correctionleft = false;
  }

  if (result.color2.lux < 1 && !correctionleft) {
    motor2.setSpeed(-0.35);
    motor1.setSpeed(0.40);
    //digitalWrite(LED_B, 1);
    correctionright = true;
  }
  else {
    motor2.setSpeed(0.40);
    //digitalWrite(LED_B, 0);
    correctionright = false;
  }
  if(!correctionleft && !correctionright) {
    distance = ultrasonic.Distance();
    if(distance <= 15) {
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
  //   motor1.setSpeed(0.4);
  //   motor2.setSpeed(0.4);
  //   correction = false;
  //   digitalWrite(LED_R, 0);
  //   digitalWrite(LED_B, 0);
  //   digitalWrite(LED_G, 1);
  // }
}