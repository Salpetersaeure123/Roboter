#include "Motor.h"

Motor::Motor(int pin1, int pin2, int pin3) {
    _pin1 = pin1;
    _pin2 = pin2;
    _pin3 = pin3;
    pinMode(_pin1, OUTPUT);
    pinMode(_pin2, OUTPUT);
    pinMode(_pin3, OUTPUT);
    digitalWrite(_pin1, LOW);
    digitalWrite(_pin2, LOW);
    digitalWrite(_pin3, LOW);
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