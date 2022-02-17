#ifndef REMOTE_CONTROL_H
#define REMOTE_CONTROL_H

#include "WiFi.h"
#include "ESP32Webserver.h"
#include "Arduino.h"
#include "sstream"
#include "Motor.h"
#include "Util.h"

enum Mode {
  NONE = -1,
  LINE = 0,
  ULTRASONIC = 1,
  REMOTE = 2
};

extern Motor motor1;
extern Motor motor2;

class RemoteControl {
    public:
        static void setup();
        static void loop(void*);
        static Mode getMode();

       static bool correction;

    private:
        static Mode mode;
        static const char* ssid[3];
        static const char* password[3];
        static ESP32WebServer server;
        static int speed;

        static void callGreeting();
        static void setDirection();
        static void setSpeed();
        static void setMode();
        static void sendResult(String content);

};

#endif
