#ifndef LINE_H
#define LINE_H

#include "Arduino.h"
#include "Sensors.h"
#include "Motor.h"
#include "Light.h"
#include "SafeMode.h"


// defines
#define SPEED_ROTATE        0.22
#define SPEED_DRIVE         0.4
#define SPEED_BACK_OFFSET   0.08

#define MIN_LIGHT_LEVEL     22
#define DISCONNECTED        70

// for color recognition
#define LIGHT_LEVEL_BLUE    37
#define LIGHT_LEVEL_NORMAL  26

#define LIGHT_LEVEL_GREEN   20

#define LIGHT_LEVEL_RED     -1

#define SIREN_LOW           440
#define SIREN_HIGH          587
#define SIREN_INTERVAL      1000

// settings
#define COUNTER_ROTATE      true
#define PRINT_VALUES        false

class Line {
    public:
        static void loop();

    private:
        // vars
        static MeassurementResult result;
        // states
        static bool correctionLeft, correctionRight;
        static bool lastRed, lastGreen, lastBlue;

};

#endif