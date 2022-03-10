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

// defines
#define DEFAULT_MODE      NONE

#define CONNECTION_TIME   1000
#define CONNECTION_TRIES  5

#define REMOTE_TIMEOUT    1000

#define BLINK_SPEED       1000

#define TWO_JOYSTICKS     false

// calculation values
#define MINIMUM_SPEED         .25
#define MAXIMUM_SPEED         1.
#define MIN_ROTATION_SPEED    70
#define MAX_ROTATION_SPEED    100
#define ROTATION_THRESHOLD    20

enum Mode {
  NONE = -1,
  LINE = 0,
  ULTRASONIC = 1,
  REMOTE = 2
};

class RemoteControl {
  public:
    static void setup();

    static Mode getMode();

    // stop motor changes when SafeMode corrects
    static bool correction;

  private:
    // vars
    static Mode mode;
    static const char* ssid[4];
    static const char* password[4];
    static const char* hostname;
    static ESP32WebServer server;
    static int speed;
    static unsigned long lastReqeust;
    static xTaskHandle hupTask;

    // processing functions
    static void setDirection();
    static void setSpeed();
    static void setMode();
    static void setSafeMode();
    static void setHupe();
    static void setLightMode();
    static void setLaser();
    
    // callback function
    static void sendResult(String content);

    // loop function
    static void loop(void*);

};

#endif
