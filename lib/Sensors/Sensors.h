#ifndef SENSORS_H
#define SENSORS_H

#include "Adafruit_TCS34725.h"
#include "Adafruit_VL53L0X.h"
#include "../Definies.h"
#include "Arduino.h"
#include "SR04.h"

//settings
#define ACTIVE_COLOR        true
#define ACTIVE_LIDAR        false
#define ACTIVE_ULTRASONIC   true

#define FORCE_INIT_COLOR    false
#define FORCE_INIT_LIDAR    false


// meassurement data of one sensor
struct Meassurement {
    uint16_t r, g, b, c, colorTemp, lux = 78;
};

// meassurement data of all sensors
struct MeassurementResult {
    Meassurement color1, color2, difference;
};

class Sensors {

public:
    static void init();

    static MeassurementResult getColorValues(bool print = false);
    static uint16_t getLidarValues(bool print = false);
    static int64_t getUltrasonicValues(bool print = false);

private:
    // instances
    static Adafruit_TCS34725 color1, color2;
    static Adafruit_VL53L0X lidar;
    static SR04 ultrasonic;
    // vars
    static MeassurementResult result;
    static boolean color1Connected;
    static boolean color2Connected;
    static boolean lidarConnected;
    static uint32_t delay;
    static uint64_t lastMeassurement;

    static void initColor();
    static void initLidar();

    static bool initTCS(Adafruit_TCS34725& sensor, TwoWire& i2c, int16_t sda=-1, int16_t scl=-1);

};

#endif