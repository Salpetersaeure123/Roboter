#ifndef REMOTE_CONTROL_H
#define REMOTE_CONTROL_H

#include "WiFi.h"
#include "ESP32Webserver.h"
#include "Arduino.h"


class RemoteControl {
    public:
        static void setup();
        static void loop();

    private:
        static const char* ssid;
        static const char* password;
        static ESP32WebServer server;

        static void callGreeting();
        static void sendResult(String content);

};

#endif
