#ifndef REMOTE_CONTROL_H
#define REMOTE_CONTROL_H

#include "WiFi.h"
#include "ESP32Webserver.h"
#include "Arduino.h"
#include "sstream"
#include "Motor.h"
#include "Util.h"

enum Mode {
  LINE = 0,
  ULTRASONIC = 1,
  REMOTE = 2
};

extern Motor motor1;
extern Motor motor2;

class RemoteControl {
    public:
        static void setup();
        static void loop();
        static Mode getMode();

    private:
        static Mode mode;
        static const char* ssid;
        static const char* password;
        static ESP32WebServer server;
        static int speed;

        static void callGreeting();
        static void setDirection();
        static void setSpeed();
        static void setMode();
        static void sendResult(String content);

};

#endif
