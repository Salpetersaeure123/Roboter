#include "Motor.h"

Motor motor1(MOTOR_LV, MOTOR_LH, MOTOR_LS);
Motor motor2(MOTOR_RV, MOTOR_RH, MOTOR_RS);

Motor::Motor(int pin1, int pin2, int pin3) {
    // store pin declarations
    _pin1 = pin1;
    _pin2 = pin2;
    _pin3 = pin3;

    // setup pins
    pinMode(_pin1, OUTPUT);
    pinMode(_pin2, OUTPUT);
    pinMode(_pin3, OUTPUT);
}

void Motor::setSpeed(int speed) {
    // store speed and direction values
    _speed = min(abs(speed), MAX_SPEED);
    _direction = (speed >= 0);

    // write outputs
    digitalWrite(_pin1, _direction ? 0 : 1);
    digitalWrite(_pin2, _direction ? 1 : 0);
    analogWrite(_pin3, _speed);
}

void Motor::setSpeed(double percent) {
    setSpeed((int)(percent*MAX_SPEED));
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
  setSpeeds(0);
}

void Motor::quarterTurn(bool left) {
  //does a 90° turn
  motor1.setSpeed((left?-1:1)*ROTATION_SPEED);
  motor2.setSpeed((left?1:-1)*ROTATION_SPEED);
  vTaskDelay(ROTATION_DURATION/4);
  setSpeeds(0);
}
