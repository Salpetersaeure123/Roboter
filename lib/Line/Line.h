#ifndef LINE_H
#define LINE_H

#include "Arduino.h"
#include "Sensors.h"
#include "Motor.h"
#include "Light.h"
#include "SafeMode.h"

class Line {
    public:
        static void loop();

    private:
        static MeassurementResult result;
        static bool correctionLeft, correctionRight;

};

#endif