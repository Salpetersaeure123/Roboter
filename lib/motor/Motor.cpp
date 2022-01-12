#include "Motor.h"

Motor::Motor(int pin1, int pin2) {
    _pin1 = pin1;
    _pin2 = pin2;
    pinMode(_pin1, OUTPUT);
    pinMode(_pin2, OUTPUT);
}

void Motor::setSpeed(int speed) {
    _speed = abs(min(speed, 255));
    _direction = speed > 0;
    digitalWrite(_direction ? _pin1 : _pin2, 0);
    analogWrite(_direction ? _pin2 : _pin1, _speed);
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