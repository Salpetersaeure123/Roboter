#include "SafeMode.h"
#include "RemoteControl.h"

bool SafeMode::enabled = true;

void SafeMode::loop() {
    // returns if safe mode disabled
    if(!enabled)
        return;
    // check if distance in front to small
    if(Sensors::getUltrasonicValues() < MIN_DISTANCE) {
        // to avoid false alerts, redo check
        if(Sensors::getUltrasonicValues() < MIN_DISTANCE) {
            // stop remote control from moving
    	    RemoteControl::correction = true;
            // start turning and giving optical and acoustic feedback
            LightManager::setBrakeLights(true);
            Speaker::startSignal();
            Motor::halfTurn();
            LightManager::setBrakeLights(false);
            Speaker::stopHupe();
            // gives control back to remote control
            RemoteControl::correction = false;
        }
    }
}

void SafeMode::setSafeMode(bool value) {
    // store value
    enabled = value;
}