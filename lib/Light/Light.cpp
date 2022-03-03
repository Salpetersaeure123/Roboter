#include "Light.h"

LightMode LightManager::_mode = AUTO;
boolean LightManager::_brems = 0;


void LightManager::setup() {
    pinMode(LASER, OUTPUT);
    pinMode(FRONT_LIGHT, OUTPUT);
    pinMode(BACK_LIGHT_L, OUTPUT);
    pinMode(BACK_LIGHT_R, OUTPUT);
    pinMode(36, INPUT);
    pinMode(39, INPUT);
    xTaskCreate(checkAutoLight, "auto light", 1024, NULL, 2, NULL);
}

#define LIGHTS_OFF  digitalWrite(FRONT_LIGHT, 0);   \
                    analogWrite(BACK_LIGHT_L, 0);  \
                    analogWrite(BACK_LIGHT_R, 0);

#define LIGHTS_ON   digitalWrite(FRONT_LIGHT, 1);   \
                    analogWrite(BACK_LIGHT_L, 70); \
                    analogWrite(BACK_LIGHT_R, 70);    

#define BREMS_LIGHT analogWrite(BACK_LIGHT_L, 255); \
                    analogWrite(BACK_LIGHT_R, 255);

void LightManager::setLightMode(LightMode mode) {
    _mode = mode;
    if(mode == OFF) {
        LIGHTS_OFF
    } else if(mode == ON) {
        LIGHTS_ON
    } else if(mode == AUTO) {
        // checkAutoLight(NULL);
    }
}

void LightManager::setLaser(boolean value) {
    digitalWrite(LASER, value);
}

void LightManager::setBremsLight(bool value) {
    _brems = value;
    if(value) {
        BREMS_LIGHT;
    } else
        setLightMode(_mode);
}

void LightManager::checkAutoLight(void*) {
    for(;;) {
        vTaskDelay(100);
        if(_mode != AUTO || _brems)
            continue;
        if(analogRead(LDR) > MIN_LIGHT_LEVEL) {
            LIGHTS_ON
        } else {
            LIGHTS_OFF
        }
    }
}