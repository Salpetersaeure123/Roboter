#include "Light.h"

LightMode LightManager::_mode = DEFAULT_LIGHT_MODE;
boolean LightManager::_brake = 0;


void LightManager::setup() {
    // init pins
    pinMode(LASER, OUTPUT);
    pinMode(FRONT_LIGHT, OUTPUT);
    pinMode(BACK_LIGHT_L, OUTPUT);
    pinMode(BACK_LIGHT_R, OUTPUT);
    pinMode(LDR, INPUT);

    // start auto light task
    xTaskCreate(checkAutoLight, "auto light", 1024, NULL, 2, NULL);
}

// Markos for setting light levels
#define LIGHTS_OFF  digitalWrite(FRONT_LIGHT, 0);   \
                    analogWrite(BACK_LIGHT_L, 0);   \
                    analogWrite(BACK_LIGHT_R, 0);

#define LIGHTS_ON   digitalWrite(FRONT_LIGHT, 1);   \
                    analogWrite(BACK_LIGHT_L, 50);  \
                    analogWrite(BACK_LIGHT_R, 50);    

#define BREMS_LIGHT analogWrite(BACK_LIGHT_L, 255); \
                    analogWrite(BACK_LIGHT_R, 255);

void LightManager::setLightMode(LightMode mode) {
    // stores mode for auto light task
    _mode = mode;

    // sets light levels
    if(mode == OFF) {
        LIGHTS_OFF
    } else if(mode == ON) {
        LIGHTS_ON
    } else if(mode == AUTO) {
        // not needed since auto light loop does it automatically
    }
}

void LightManager::setLaser(boolean value) {
    digitalWrite(LASER, value);
}

void LightManager::setBrakeLights(bool value) {
    // stores value for auto light task
    _brake = value;

    //set brems lights
    if(value) {
        BREMS_LIGHT;
    } else
        setLightMode(_mode);
}

void LightManager::checkAutoLight(void*) {
    for(;;) {
        // 10 executions per second
        vTaskDelay(100);

        // return if auto light isn't enabled
        if(_mode != AUTO)
            continue;

        // checks LDR value
        if(analogRead(LDR) > MIN_LIGHT_LEVEL) {
            // if brake light activ only overrite front light
            if(_brake)
                digitalWrite(FRONT_LIGHT, 1);
            else
                LIGHTS_ON
        } else {
            // if brake light activ only overrite front light
            if(_brake)
                digitalWrite(FRONT_LIGHT, 0);
            else
                LIGHTS_OFF
        }
    }
}