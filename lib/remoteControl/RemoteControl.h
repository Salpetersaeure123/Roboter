#ifndef REMOTE_CONTROL_H
#define REMOTE_CONTROL_H

#include "WiFi.h"
#include "ESP32Webserver.h"
#include "Arduino.h"
#include "sstream"
#include "Motor.h"
#include "Util.h"
#include "Speaker.h"
#include "Light.h"
#include "SafeMode.h"

enum Mode {
  NONE = -1,
  LINE = 0,
  ULTRASONIC = 1,
  REMOTE = 2
};

class RemoteControl {
    public:
        static void setup();
        static void loop(void*);

        static Mode getMode();

       static bool correction;

    private:
        static Mode mode;
        static const char* ssid[4];
        static const char* password[4];
        static const char* hostname;
        static ESP32WebServer server;
        static int speed;
        static unsigned long lastReqeust;

        static void setDirection();
        static void setSpeed();
        static void setMode();
        static void setSafeMode();
        static void setHupe();
        static void setLightMode();
        static void setLaser();

        static void sendResult(String content);

};

#endif
