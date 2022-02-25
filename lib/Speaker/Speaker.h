#ifndef SPEAKER_H
#define SPEAKER_H

#include "Arduino.h"
#include "../Definies.h"

class Speaker {
    public:
        static void setup();
        static void hupe(double d);
        static void signal();

    private:

};

#endif