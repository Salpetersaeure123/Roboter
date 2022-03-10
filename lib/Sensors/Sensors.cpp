#include "Sensors.h"

Adafruit_TCS34725 Sensors::color1 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_16X); 
Adafruit_TCS34725 Sensors::color2 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_16X);
Adafruit_VL53L0X Sensors::lidar = Adafruit_VL53L0X();
SR04 Sensors::ultrasonic = SR04(SONIC_ECHO, SONIC_TRIG);
MeassurementResult Sensors::result;
boolean Sensors::color1Connected;
boolean Sensors::color2Connected;
boolean Sensors::lidarConnected;
uint32_t Sensors::delay;
uint64_t Sensors::lastMeassurement;

void Sensors::init() {
    // init sensors
    initColor();
    initLidar();
}

void Sensors::initColor() {
    // returns if color senors not activaed
    if(!ACTIVE_COLOR)
        return;
    // init both color senors and stores connection status
    color1Connected = initTCS(color1, I2C_1, SDA_1, SCL_1);
    color2Connected = initTCS(color2, I2C_2, SDA_2, SCL_2);
}

void Sensors::initLidar() {
    // returns if lidar not activated
    if(!ACTIVE_LIDAR)
        return;
    // starts i2c connection
    if(!Wire1.begin(SDA_2, SCL_2)) {
        // i2c connection failed
        if(DEBUG_LIDAR) {
            Serial.print(F("I2C Connection failed.\t"));
            Serial.print(F("retry: ")); Serial.println(F(FORCE_INIT_LIDAR?"true":"false"));
        }
        // prevent further running until connection established
        while(FORCE_INIT_LIDAR&&!Wire1.begin(SDA_2, SCL_2));
    }

    // var for connection status
    bool initSuccessfull = true;
    // try connection to sensor
    if (!lidar.begin(41U, true, &Wire1)) {
        // lidar connection failed
        if(DEBUG_LIDAR) {
            Serial.print(F("Failed to boot VL53L0X.\t"));
            Serial.print(F("retry: ")); Serial.println(F(FORCE_INIT_LIDAR?"true":"false"));
        }
        initSuccessfull = false;
        // prevent further running until connection established
        while(FORCE_INIT_LIDAR&&!lidar.begin());
    }

    // if connected, set lidar settings
    if(lidar.Status!=-5)
        lidar.setMeasurementTimingBudgetMicroSeconds(10000);

    // store connection status
    lidarConnected = initSuccessfull||FORCE_INIT_LIDAR;

    if(DEBUG_LIDAR) {
        Serial.print(F("Adafruit VL53L0X init ")); Serial.println(F(lidar.Status!=-5?"successfull":"failed"));
    }
}

bool Sensors::initTCS(Adafruit_TCS34725& sensor, TwoWire& i2c, int16_t sda, int16_t scl) {
    // set i2c pins if changed
    if(sda != -1 && scl != -1)
        i2c.setPins(sda, scl);

    // var for connection status
    bool initSuccessfull = true;
    // try connection to sensor
    if(!sensor.begin(TCS34725_ADDRESS, &i2c)) {
        // Color sensor connection failed
        if(DEBUG_COLOR) {
            Serial.print(F("No TCS34725 found ... check your connections.\t"));
            Serial.print(F("retry: ")); Serial.println(F(FORCE_INIT_COLOR?"true":"false"));
        }
        initSuccessfull = false;
        // prevent further running until connection established
        while(FORCE_INIT_COLOR&&!sensor.begin(TCS34725_ADDRESS, &i2c));   
    }

    if(DEBUG_COLOR) {
        Serial.print(F("Adafruit TCS34725 init ")); Serial.println(F(initSuccessfull||FORCE_INIT_COLOR?"successfull":"failed"));
    }
    // returns connection status
    return initSuccessfull||FORCE_INIT_COLOR;
}

MeassurementResult Sensors::getColorValues(bool print) {
    // returns if not activated or connected
    if(!ACTIVE_COLOR)
        return result;
    if(!color1Connected||!color2Connected)
        return result;

    // get color values
    color1.getRawDataDirect(&result.color1.r, &result.color1.g, &result.color1.b, &result.color1.c);
    color2.getRawDataDirect(&result.color2.r, &result.color2.g, &result.color2.b, &result.color2.c);
    	
    // calculate some values
    result.color1.colorTemp = color1.calculateColorTemperature(result.color1.r, result.color1.g, result.color1.b);
    result.color2.colorTemp = color2.calculateColorTemperature(result.color2.r, result.color2.g, result.color2.b);
    result.color1.lux = color1.calculateLux(result.color1.r, result.color1.g, result.color1.b);
    result.color2.lux = color2.calculateLux(result.color2.r, result.color2.g, result.color2.b);

    // calculate differences
    result.difference.r = result.color1.r-result.color2.r;
    result.difference.g = result.color1.g-result.color2.g;
    result.difference.b = result.color1.b-result.color2.b;
    result.difference.c = result.color1.c-result.color2.c;
    result.difference.lux = result.color1.lux-result.color2.lux;
    result.difference.colorTemp = result.color1.colorTemp-result.color2.colorTemp;

    // print out senor values
    if(DEBUG_COLOR && print) {
        Serial.print("Color 1: ");
        Serial.print("Color Temp: "); Serial.print(result.color1.colorTemp, DEC); Serial.print(" K - ");
        Serial.print("Lux: "); Serial.print(result.color1.lux, DEC); Serial.print(" - ");
        Serial.print("R: "); Serial.print(result.color1.r, DEC); Serial.print(" ");
        Serial.print("G: "); Serial.print(result.color1.g, DEC); Serial.print(" ");
        Serial.print("B: "); Serial.print(result.color1.b, DEC); Serial.print(" ");
        Serial.print("C: "); Serial.print(result.color1.c, DEC); Serial.print(" ");
        Serial.print("\t");
        
        Serial.print("Color 2: ");
        Serial.print("Color Temp: "); Serial.print(result.color2.colorTemp, DEC); Serial.print(" K - ");
        Serial.print("Lux: "); Serial.print(result.color2.lux, DEC); Serial.print(" - ");
        Serial.print("R: "); Serial.print(result.color2.r, DEC); Serial.print(" ");
        Serial.print("G: "); Serial.print(result.color2.g, DEC); Serial.print(" ");
        Serial.print("B: "); Serial.print(result.color2.b, DEC); Serial.print(" ");
        Serial.print("C: "); Serial.print(result.color2.c, DEC); Serial.print(" ");
        Serial.print("\t");
        
        Serial.print("Difference: ");
        Serial.print("Color Temp: "); Serial.print(result.difference.colorTemp, DEC); Serial.print(" K - ");
        Serial.print("Lux: "); Serial.print(result.difference.lux, DEC); Serial.print(" - ");
        Serial.print("R: "); Serial.print(result.difference.r, DEC); Serial.print(" ");
        Serial.print("G: "); Serial.print(result.difference.g, DEC); Serial.print(" ");
        Serial.print("B: "); Serial.print(result.difference.b, DEC); Serial.print(" ");
        Serial.print("C: "); Serial.print(result.difference.c, DEC); Serial.print(" ");
        Serial.println();
    }

    // return data
    return result;
}

uint16_t Sensors::getLidarValues(bool print) {
    // returns if not activated or connected
    if(!ACTIVE_LIDAR)
        return -1;
    if(!lidarConnected)
        return -1;

    // get distance value
    VL53L0X_RangingMeasurementData_t meassure;
    lidar.rangingTest(&meassure, false);

    // check if out of range
    if(meassure.RangeStatus == 4) {
        if(DEBUG_LIDAR && print)
            Serial.println(F("Lidar out of range"));
    } else {
        // print distance
        if(DEBUG_LIDAR && print)
            Serial.println("Lidar distance: "+String(meassure.RangeMilliMeter)+"mm");
    }
    // return distance
    return meassure.RangeMilliMeter;
}

int64_t Sensors::getUltrasonicValues(bool print) {
    // returns if not active
    if(!ACTIVE_ULTRASONIC)
        return -1;

    // get distance value
    long distance = ultrasonic.Distance();

    // print distance
    if(DEBUG_ULTRASONIC && print)
        Serial.println("Ultrasonic distance: "+String(distance)+"cm");

    // return distance
    return distance;
}