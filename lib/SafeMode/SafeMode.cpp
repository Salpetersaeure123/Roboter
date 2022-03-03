#include "SafeMode.h"

bool SafeMode::enabled = true;

void SafeMode::loop() {
    if(!enabled)
        return;
    if(Sensors::getUltrasonicValues() < 15) {
        LightManager::setBremsLight(true);
        Motor::setSpeeds(0);
        long start = millis();
        while(millis()-start < 1000) {
            Speaker::signal();
        }
        Motor::halfTurn();
        LightManager::setBremsLight(false);
    }
}

void SafeMode::setSafeMode(bool value) {
    enabled = value;
}