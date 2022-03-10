#ifndef SPEAKER_H
#define SPEAKER_H

#include "Arduino.h"
#include "../Definies.h"

class Speaker {
    public:
        static void setup();

        // have to be constantly executed
        static void hupe(double d);
        static void signal();

        // have to be started
        static void startHupe();
        static void startSignal();
        static void startSignal2();

        // stop function
        static void stopHupe();

    private:

};

#endif