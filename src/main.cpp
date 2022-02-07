#include "Motor.h"
#include "Sensors.h"
#include "SR04.h"
#include "RemoteControl.h"

Motor motor1(18, 19);
Motor motor2(17, 16);

#define LED_R 23
#define LED_G 22
#define LED_B 21

#define LINE_MODE 0
#define ULTRASONIC_MODE 1
#define REMOTE_MODE 2
#define MODE REMOTE_MODE

SR04 ultrasonic = SR04(SONIC_ECHO, SONIC_TRIG);
long distance;

void setup() {
#if DEBUG
  Serial.begin(115200);
#endif

#if MODE == LINE_MODE
  Sensors::init();
#elif MODE == ULTRASONIC_MODE
  // ultrasonic setup siehe oben
  Serial.println("ultrasonic setup done");
#elif MODE == REMOTE_MODE
  RemoteControl::setup();
#endif

  motor1.setSpeed(.4);
  motor2.setSpeed(.4);

  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
}

bool correctionleft = false;
bool correctionright = false;

void lineLoop();
void ultrasonicLoop();

void loop() {
#if MODE == LINE_MODE
  lineLoop();
#elif MODE == ULTRASONIC_MODE
  ultrasonicLoop();
  Serial.println("will execute ultrasonicLoop");
#endif
}

void ultrasonicLoop() {
  //Serial.println("executing ultrasonicLoop");
  distance = ultrasonic.Distance();
  if (distance <= 50) {
    //stop
    motor1.setSpeed(0);
    motor2.setSpeed(0);
    while (distance <= 50)
    {
      distance = ultrasonic.Distance();
    }
    motor1.setSpeed(150);
    motor2.setSpeed(150);
  }
}

void lineLoop() {
  MeassurementResult result;
  result = Sensors::getColorValues(false);
  // Sensors::getLidarValues(true);
  if (result.color1.lux < 1 && !correctionright) {
    motor1.setSpeed(-0.4);
    motor2.setSpeed(0.4);
    digitalWrite(LED_R, 1);
    correctionleft = true;
  } else {
    digitalWrite(LED_R, 0);
    motor1.setSpeed(0.58);
    correctionleft = false;
  }
  if (result.color2.lux < 1 && !correctionleft) {
    motor2.setSpeed(-0.4);
    motor1.setSpeed(0.4);
    digitalWrite(LED_B, 1);
    correctionright = true;
  } else {
    motor2.setSpeed(0.58);
    digitalWrite(LED_B, 0);
    correctionright = false;
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