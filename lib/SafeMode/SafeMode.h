#ifndef SAFEMODE_H
#define SAFEMODE_H

#include "Sensors.h"
#include "Speaker.h"
#include "Light.h"
#include "Motor.h"

// defines
#define MIN_DISTANCE    20

class SafeMode {
    public:
        static void loop();
        static void setSafeMode(bool value);

    private:
        static bool enabled;
};

#endif