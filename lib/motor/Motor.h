#ifndef MOTOR_H
#define MOTOR_H

#include "Arduino.h"
#include "analogWrite.h"
#include "../Definies.h"
#include "Speaker.h"

// defines
#define ROTATION_SPEED      0.33
#define ROTATION_DURATION   1700

#define MAX_SPEED           255

class Motor;

// Motor objects
extern Motor motor1; // left
extern Motor motor2; // right

class Motor {
    public: 
        Motor(int pinLeft, int pinRight, int pinEnable);

        void setSpeed(int speed);
        void setSpeed(double percent);
        int getSpeed();
        bool getDirection();
        
        // static functions to set both motors
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
        // vars
        int _pin1, _pin2, _pin3;
        int _speed;
        bool _direction;
};

#endif