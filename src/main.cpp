#include "Motor.h"
#include "Sensors.h"
#include "SR04.h"

Motor motor1(18, 19); // = left
Motor motor2(17, 16); // = right

#define LED_R 23
#define LED_G 22
#define LED_B 21

#define LINE_MODE 1
#define ultrasonic_MODE 0
#define MODE LINE_MODE

SR04 ultrasonic = SR04(SONIC_ECHO, SONIC_TRIG);
long distance;

void setup() {
#if DEBUG
  Serial.begin(115200);
#endif

#if MODE == LINE_MODE
  Sensors::init();

  motor1.setSpeed(0.4);
  motor2.setSpeed(0.4);

  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  #endif
}

bool correctionleft = false;
bool correctionright = false;

void halfTurn();
void fullTurn();
void lineLoop();
void ultrasonicLoop();

void loop() {
#if MODE == LINE_MODE
  lineLoop();
#endif
#if MODE == ultrasonic_MODE
  ultrasonicLoop();
#endif
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

void ultrasonicLoop() {
  distance = ultrasonic.Distance();
  if(distance < 20) {
    halfTurn();
  }
  // only for tests
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