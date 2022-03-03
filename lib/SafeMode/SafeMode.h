#ifndef SAFEMODE_H
#define SAFEMODE_H

#include "Sensors.h"
#include "Speaker.h"
#include "Light.h"
#include "Motor.h"

class SafeMode {
    public:
        static void loop();
        static void setSafeMode(bool value);

    private:
        static bool enabled;
};

#endif