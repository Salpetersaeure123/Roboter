#ifndef SENSORS_H
#define SENSORS_H

#include "Adafruit_TCS34725.h"
#include "Adafruit_VL53L0X.h"
#include "../Definies.h"
#include "Arduino.h"

class Sensors {

public:
    static void init();

    static void printColorValues();
    static void printLidarValues();

private:
    static Adafruit_TCS34725 color1, color2;
    static Adafruit_VL53L0X lidar;

    static void initColor();
    static void initLidar();

    static void initTCS(Adafruit_TCS34725& sensor, TwoWire& i2c, int16_t sda=-1, int16_t scl=-1);

};

#endif