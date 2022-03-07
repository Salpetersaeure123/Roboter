#ifndef SPEAKER_H
#define SPEAKER_H

#include "Arduino.h"
#include "../Definies.h"

class Speaker {
    public:
        static void setup();
        static void hupe(double d);
        static void signal();
        static void startHupe();
        static void stopHupe();
        static void startSignal();
        static void startSignal2();


    private:

};

#endif