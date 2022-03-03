#include "Motor.h"

#define ROTATION_SPEED      0.3
#define ROTATION_DURATION   1700

Motor motor1(MOTOR_LV, MOTOR_LH, MOTOR_LS); // = left
Motor motor2(MOTOR_RV, MOTOR_RH, MOTOR_RS); // = rights

Motor::Motor(int pin1, int pin2, int pin3) {
    _pin1 = pin1;
    _pin2 = pin2;
    _pin3 = pin3;
    pinMode(_pin1, OUTPUT);
    pinMode(_pin2, OUTPUT);
    pinMode(_pin3, OUTPUT);
    // digitalWrite(_pin1, LOW);
    // digitalWrite(_pin2, LOW);
    // digitalWrite(_pin3, LOW);
}

void Motor::setSpeed(int speed) {
    _speed = min(abs(speed), 255);
    _direction = (speed >= 0);
    // analogWrite((_direction ? _pin1 : _pin2), 0);
    // analogWrite((_direction ? _pin2 : _pin1), _speed);
    digitalWrite((_direction ? _pin1 : _pin2), 0);
    digitalWrite((_direction ? _pin2 : _pin1), 1);
    analogWrite(_pin3, _speed);
}

void Motor::setSpeed(double percent) {
    setSpeed((int)(percent*255));
}

int Motor::getSpeed() {
    return _speed;
}

bool Motor::getDirection() {
    return _direction;
}

void Motor::setRotation(int speed) {
    motor1.setSpeed(-speed);
    motor2.setSpeed(speed);
}
void Motor::setRotation(double speed) {
    motor1.setSpeed(-speed);
    motor2.setSpeed(speed);
}

void Motor::setSpeeds(int left, int right) {
    motor1.setSpeed(left);
    motor2.setSpeed(right);
}

void Motor::setSpeeds(double left, double right) {
    motor1.setSpeed(left);
    motor2.setSpeed(right);
}

void Motor::setSpeeds(int both) {
    motor1.setSpeed(both);
    motor2.setSpeed(both);
}
void Motor::setSpeeds(double both) {
    motor1.setSpeed(both);
    motor2.setSpeed(both);
}

void Motor::fullTurn(bool left) {
  //does a 360° turn
  motor1.setSpeed((left?-1:1)*ROTATION_SPEED);
  motor2.setSpeed((left?1:-1)*ROTATION_SPEED);
  vTaskDelay(ROTATION_DURATION);
  setSpeeds(0);
}

void Motor::halfTurn(bool left) {
  //does a 180° turn
  motor1.setSpeed((left?-1:1)*ROTATION_SPEED);
  motor2.setSpeed((left?1:-1)*ROTATION_SPEED);
  vTaskDelay(ROTATION_DURATION/2);
//   long start = millis();
//   while(millis()-start < ROTATION_DURATION/2) {
//       Speaker::signal();
//   }
  setSpeeds(0);
}

void Motor::quarterTurn(bool left) {
  //does a 90° turn
  motor1.setSpeed((left?-1:1)*ROTATION_SPEED);
  motor2.setSpeed((left?1:-1)*ROTATION_SPEED);
  vTaskDelay(ROTATION_DURATION/4);
  setSpeeds(0);
}
