#ifndef MOTOR_H
#define MOTOR_H

#include "Arduino.h"
#include "analogWrite.h"

class Motor {
    public: 
        Motor(int pin1, int pin2);
        void setSpeed(int speed);
        void setSpeed(double percent);
        int getSpeed();
        bool getDirection();

    private:
        int _pin1;
        int _pin2;
        int _speed;
        bool _direction;
};

#endif