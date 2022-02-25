#ifndef MOTOR_H
#define MOTOR_H

#include "Arduino.h"
#include "analogWrite.h"
#include "../Definies.h"

class Motor;

extern Motor motor1;
extern Motor motor2;

class Motor {
    public: 
        Motor(int pin1, int pin2, int pin3);
        void setSpeed(int speed);
        void setSpeed(double percent);
        int getSpeed();
        bool getDirection();
        
        static void setRotation(int speed);
        static void setRotation(double speed);
        static void setSpeeds(int left, int right);
        static void setSpeeds(double left, double right);
        static void setSpeeds(int both);
        static void setSpeeds(double both);
        static void fullTurn(bool left = true);
        static void halfTurn(bool left = true);
        static void quarterTurn(bool left = true);

    private:
        int _pin1;
        int _pin2;
        int _pin3;
        int _speed;
        bool _direction;
};

#endif