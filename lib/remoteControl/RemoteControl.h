#ifndef REMOTE_CONTROL_H
#define REMOTE_CONTROL_H

#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "Arduino.h"


class RemoteControl {
    public:
        static void setup();

    private:
        static const char* ssid;
        static const char* password;
        static AsyncWebServer server;

};

#endif
