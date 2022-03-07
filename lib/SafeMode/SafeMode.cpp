#include "SafeMode.h"
#include "RemoteControl.h"

bool SafeMode::enabled = true;

void SafeMode::loop() {
    if(!enabled)
        return;
    if(Sensors::getUltrasonicValues() < 20) {
        if(Sensors::getUltrasonicValues() < 20) {
    	    RemoteControl::correction = true;
            LightManager::setBremsLight(true);
            // Motor::setSpeeds(0);
            // long start = millis();
            // while(millis()-start < 1000) {
            //     Speaker::signal();
            // }
            Speaker::startSignal();
            Motor::halfTurn();
            LightManager::setBremsLight(false);
            Speaker::stopHupe();
            RemoteControl::correction = false;
        }
    }
}

void SafeMode::setSafeMode(bool value) {
    enabled = value;
}