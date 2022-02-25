#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "Arduino.h"
#include "Sensors.h"
#include "Motor.h"
#include "RemoteControl.h"

class Ultrasonic {
    public:
        static void loop();
        static void loop2();

    private:
        static int fourDistances[4];
        static int direction;
        static long distance;


};

#endif