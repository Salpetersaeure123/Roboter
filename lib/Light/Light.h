#ifndef LIGHT_H
#define LIGHT_H

#include "Arduino.h"
#include "../Definies.h"
#include "analogWrite.h"


#define MIN_LIGHT_LEVEL 800


enum LightMode {
  OFF = -1,
  AUTO = 0,
  ON = 1
};

class LightManager {
    public:
        static void setup();
        static void setLightMode(LightMode);
        static void setLaser(bool value);
        static void setBremsLight(bool value);

    private:
        static LightMode _mode;
        static boolean _brems;

        static void checkAutoLight(void*);
};

#endif