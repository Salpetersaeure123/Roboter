#ifndef SENSORS_H
#define SENSORS_H

#include "Adafruit_TCS34725.h"
#include "Adafruit_VL53L0X.h"
#include "../Definies.h"
#include "Arduino.h"

struct Meassurement {
    uint16_t r, g, b, c, colorTemp, lux;
};

struct MeassurementDiff {
    int16_t r, g, b, c, colorTemp, lux;
};

struct MeassurementResult {
    Meassurement color1, color2;
    MeassurementDiff difference;
};

class Sensors {

public:
    static void init();

    static MeassurementResult getColorValues(bool print = false);
    static uint16_t getLidarValues(bool print = false);

private:
    static Adafruit_TCS34725 color1, color2;
    static Adafruit_VL53L0X lidar;
    static MeassurementResult result;
    static uint32_t delay;
    static uint64_t lastMeassurement;

    static void initColor();
    static void initLidar();

    static void initTCS(Adafruit_TCS34725& sensor, TwoWire& i2c, int16_t sda=-1, int16_t scl=-1);

};

#endif