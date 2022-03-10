#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "Arduino.h"
#include "Sensors.h"
#include "Motor.h"
#include "RemoteControl.h"

// defines
#define SPEED_OFF       0
#define SPEED_DRIVE     .4
#define SPEED_FAST      .7
#define SPEED_ROTATE    .27

#define MIN_DISTANCE    40
#define SEARCH_DURATION 4*1000
#define LEAVE_DISTANCE  100


class Ultrasonic {
    public:
        // does quarter turns
        static void loop();
        // meassures continuesly
        static void loop2();

    private:
        // vars
        static int fourDistances[4];
        static int direction;
        static long distance;


};

#endif