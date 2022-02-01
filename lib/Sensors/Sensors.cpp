#include "Sensors.h"

Adafruit_TCS34725 Sensors::color1 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X); 
Adafruit_TCS34725 Sensors::color2 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);
Adafruit_VL53L0X Sensors::lidar = Adafruit_VL53L0X();

void Sensors::init() {
    initColor();
    initLidar();
}

void Sensors::initColor() {
    initTCS(color1, I2C_1, SDA_1, SCL_1);
    initTCS(color2, I2C_2, SDA_2, SCL_2);
}

void Sensors::initLidar() {
    if(!Wire.begin(SDA_1, SCL_1)) {
        if(DEBUG_LIDAR) {
            Serial.print(F("I2C Connection failed.\t"));
            Serial.print(F("retry: ")); Serial.println(F(FORCE_INIT_LIDAR?"true":"false"));
        }
    while(FORCE_INIT_LIDAR&&!Wire.begin(SDA_1, SCL_1));
    }
    if (!lidar.begin()) {
        if(DEBUG_LIDAR) {
        Serial.print(F("Failed to boot VL53L0X.\t"));
        Serial.print(F("retry: ")); Serial.println(F(FORCE_INIT_LIDAR?"true":"false"));
        }
        while(FORCE_INIT_LIDAR&&!lidar.begin());
    }
    if(lidar.Status!=-5)
        lidar.setMeasurementTimingBudgetMicroSeconds(10000);
    if(DEBUG_LIDAR) {
        Serial.print(F("Adafruit VL53L0X init ")); Serial.println(F(lidar.Status!=-5?"successfull":"failed"));
    }
}

void Sensors::initTCS(Adafruit_TCS34725& sensor, TwoWire& i2c, int16_t sda, int16_t scl) {
    if(sda != -1 && scl != -1)
        i2c.setPins(sda, scl);
    bool initSuccessfull = true;
    if(!sensor.begin(TCS34725_ADDRESS, &i2c)) {
        if(DEBUG_COLOR) {
            Serial.print(F("No TCS34725 found ... check your connections.\t"));
            Serial.print(F("retry: ")); Serial.println(F(FORCE_INIT_COLOR?"true":"false"));
        }
        initSuccessfull = false;
        while(FORCE_INIT_COLOR&&!sensor.begin(TCS34725_ADDRESS, &i2c));   
    }
    if(DEBUG_COLOR) {
        Serial.print(F("Adafruit TCS34725 init ")); Serial.println(F(initSuccessfull||FORCE_INIT_COLOR?"successfull":"failed"));
    }
}

void Sensors::printColorValues() {
    uint16_t r1, g1, b1, c1, colorTemp1, lux1, r2, g2, b2, c2, colorTemp2, lux2;

    color1.getRawData(&r1, &g1, &b1, &c1);
    color2.getRawData(&r2, &g2, &b2, &c2);

    // colorTemp = tcs.calculateColorTemperature(r, g, b);
    // colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
    lux1 = color1.calculateLux(r1, g1, b1);
    lux2 = color2.calculateLux(r2, g2, b2);

    Serial.print("Color 1: ");
    Serial.print("Color Temp: "); Serial.print(colorTemp1, DEC); Serial.print(" K - ");
    Serial.print("Lux: "); Serial.print(lux1, DEC); Serial.print(" - ");
    Serial.print("R: "); Serial.print(r1, DEC); Serial.print(" ");
    Serial.print("G: "); Serial.print(g1, DEC); Serial.print(" ");
    Serial.print("B: "); Serial.print(b1, DEC); Serial.print(" ");
    Serial.print("C: "); Serial.print(c1, DEC); Serial.print(" ");
    Serial.print("\t");
    
    Serial.print("Color 2: ");
    Serial.print("Color Temp: "); Serial.print(colorTemp2, DEC); Serial.print(" K - ");
    Serial.print("Lux: "); Serial.print(lux2, DEC); Serial.print(" - ");
    Serial.print("R: "); Serial.print(r2, DEC); Serial.print(" ");
    Serial.print("G: "); Serial.print(g2, DEC); Serial.print(" ");
    Serial.print("B: "); Serial.print(b2, DEC); Serial.print(" ");
    Serial.print("C: "); Serial.print(c2, DEC); Serial.print(" ");
    Serial.print("\t");
    
    Serial.print("Difference: ");
    Serial.print("Color Temp: "); Serial.print(colorTemp2-colorTemp1, DEC); Serial.print(" K - ");
    Serial.print("Lux: "); Serial.print(lux2-lux1, DEC); Serial.print(" - ");
    Serial.print("R: "); Serial.print(r2-r1, DEC); Serial.print(" ");
    Serial.print("G: "); Serial.print(g2-g1, DEC); Serial.print(" ");
    Serial.print("B: "); Serial.print(b2-b1, DEC); Serial.print(" ");
    Serial.print("C: "); Serial.print(c2-c1, DEC); Serial.print(" ");
    Serial.println();
}

void Sensors::printLidarValues() {
    VL53L0X_RangingMeasurementData_t measure;

    lidar.rangingTest(&measure, false);

    if(measure.RangeStatus == 4) {
        if(DEBUG_LIDAR)
            Serial.println(F("Lidar out of range"));
    } else {
        if(DEBUG_LIDAR)
            Serial.println("Lidar distance: "+String(measure.RangeMilliMeter)+"mm");
    }
}