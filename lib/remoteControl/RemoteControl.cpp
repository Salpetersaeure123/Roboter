#include "RemoteControl.h"

ESP32WebServer RemoteControl::server(80);
unsigned long RemoteControl::lastReqeust = UINT32_MAX;
Mode RemoteControl::mode = DEFAULT_MODE;
int RemoteControl::speed = 0;
bool RemoteControl::correction = false;
xTaskHandle RemoteControl::hupTask = NULL;

// Wifi settings
const char* RemoteControl::ssid[4] = {"ESP-Bot", "JCBS-Schüler", "NetFrame", "ESP32"};
const char* RemoteControl::password[4] = {"12345678", "S1,16DismdEn;deieKG!", "87934hzft9oeu4389nv8o437893hf978", "12345678"};
const char* RemoteControl::hostname = "ESP32_Bot";


void RemoteControl::setup() {
    // setup WiFi
    // set wifi settings
    WiFi.mode(WIFI_AP_STA);
    WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
    WiFi.setHostname(hostname);

    // goes through known wifi networks
    for(int i = 1; i < sizeOf(ssid); i++) {
        // tires to connect
        WiFi.begin(ssid[i%sizeOf(ssid)], password[i%sizeOf(password)]);
        for(int j = 0; j < CONNECTION_TRIES; j++) {
            Serial.println("Connecting to "+String(ssid[i])+"...");
            delay(CONNECTION_TIME);

            // if connected continues
            if(WiFi.status() == WL_CONNECTED)
                break;
        }
        // if connected continues
        if(WiFi.status() == WL_CONNECTED)
            break;
    }

    // if connected print IP
    if(WiFi.status() == WL_CONNECTED)
        Serial.println(WiFi.localIP());

    // if not connected start own wifi
    else {
        WiFi.softAP(ssid[0], password[0]);
        // print IP
        Serial.println(WiFi.softAPIP());
    }

    // setup server
    // set http commands
    server.on("/direction", setDirection);
    server.on("/speed", setSpeed);
    server.on("/mode", setMode);
    server.on("/hupe", setHupe);
    server.on("/lightMode", setLightMode);
    server.on("/safeMode", setSafeMode);
    server.on("/laser", setLaser);
        
    server.begin();
    Serial.println("Server starteted");

    // start loop task
    xTaskCreate(RemoteControl::loop, "remote", 4*1024, NULL, 5, NULL);
}
 
void RemoteControl::loop(void*) { 
    // vars
    unsigned long lastBlink = 0; 
    bool blinkState = 0;

    for(;;) {
        // if not connected or no reponce
        if(lastReqeust == UINT32_MAX || (mode == REMOTE && millis()-lastReqeust > REMOTE_TIMEOUT)) {
            // if no responce
            if(lastReqeust != UINT32_MAX) {
                // stop driving to prevent crashes
                Motor::setSpeeds(0);
                lastReqeust = UINT32_MAX;
            }

            // blink
            if(millis()-lastBlink > BLINK_SPEED) {
                lastBlink = millis();
                blinkState = !blinkState;
                LightManager::setBrakeLights(blinkState);
            }
        }

        // check if new requests arrived
        server.handleClient();

        // delay to give some proccess time to other loops
        vTaskDelay(5);
    }
}

Mode RemoteControl::getMode() {
    return mode;
}

void RemoteControl::setDirection() {
    // returns if not in remote control mode or if SafeMode intervenes
    if(mode != REMOTE || correction) {
        sendResult("Direction not changed");
        return;
    }

    // reset last request
    lastReqeust = millis();

    // create paramter vars
    int angle = 0;
    int strength = 0;

    // get paramters
    for (int i = 0; i < server.args(); i++)
        if(server.argName(i).equals("angle")) {
            // convert string to int
            std::string s = server.arg(i).c_str();
            std::stringstream intValue(s);
            intValue >> angle;            
        } else if(server.argName(i).equals("strength")) {
            // convert string to int
            std::string s = server.arg(i).c_str();
            std::stringstream intValue(s);
            intValue >> strength;            
        } else if(server.argName(i).equals("speed")) {
            // convert string to int
            std::string s = server.arg(i).c_str();
            std::stringstream intValue(s);
            intValue >> speed;            
        }


    // calculate direction
    int a = (180-angle%180)*PI/180;
    int x = TWO_JOYSTICKS? (angle==0?-1:1)*strength : cos(a)*strength;
    int y = sin(a)*strength;

    // set break lights
    if(speed < 0)
        LightManager::setBrakeLights(true);
    else
        LightManager::setBrakeLights(false);

    // map speed 
    int mappedSpeed;   
    if(speed != 0)
        mappedSpeed = (speed<0?speed/abs(speed):1)*map(abs(speed), 0, 100, floor(MINIMUM_SPEED*255), round(MAXIMUM_SPEED*255));

    // motor speed vars
    double _speed = 0;
    double _speed2 = 0;

    // do on point rotation if speed is zero
    if(mappedSpeed == 0) {
        // and if deflection greater 20
        if(abs(x)>ROTATION_THRESHOLD) {
            _speed = map(abs(x), ROTATION_THRESHOLD, 100, MIN_ROTATION_SPEED, MAX_ROTATION_SPEED)/255.;
            _speed2 = -_speed;
        }
    } else {
        // calculate deflection
        double v = 1-abs(cos(a)*strength/100.);
        // calculate base speed
        _speed = mappedSpeed/255.;
        // set base speed to zero if it is to small for the motors to move
        if(abs(_speed) < MINIMUM_SPEED)
            _speed= 0.; 
        // calculate reduced speed based on the deflection
        _speed2 = v*_speed;
        // map reduced speed to actual range
        _speed2 = (_speed2<0?_speed2/abs(_speed2):1)*MAP(abs(_speed2), 0., abs(_speed), MINIMUM_SPEED, abs(_speed));
        // set reduced speed to zero if it is to small for the motors to move
        if(abs(_speed2) < MINIMUM_SPEED)
            _speed2 = 0.;
    }

    // writes speeds to the respective motors (dependant on whitch side the joystick is, the reduced speed is assigned)
    if(x>0) {
        motor1.setSpeed(_speed2);
        motor2.setSpeed(_speed);
    } else {
        motor1.setSpeed(_speed);
        motor2.setSpeed(_speed2);
    }

    sendResult("Direction changed");
}

void RemoteControl::setSpeed() {
    // get parameters
    for (int i = 0; i < server.args(); i++)
        if(server.argName(i).equals("strength")) {
            // cast string to int
            std::string s = server.arg(i).c_str();
            std::stringstream intValue(s);
            intValue >> speed;
            // map speed value accordingly
            speed=map(speed, 0, 100, 100, 255);           
        }
    
    sendResult("Speed changed");
}

void RemoteControl::setMode() {
    // get parameters
    for (int i = 0; i < server.args(); i++)
        if(server.argName(i).equals("mode")) {
            // cast string to int
            std::string s = server.arg(i).c_str();
            std::stringstream intValue(s);
            int value = 0;
            intValue >> value;
            mode = (Mode) value;
            Serial.println(mode);

            // reset parms
            motor1.setSpeed(0);
            motor2.setSpeed(0);
            LightManager::setBrakeLights(false);
        }
    
    sendResult("Mode set to "+String(mode)+"");
}

void RemoteControl::setSafeMode() {
    // get parameters
    for(int i = 0; i < server.args(); i++)
        if(server.argName(i).equals("mode")) {
            // set safe mode
            if(server.arg(i).equals("true")) {
                SafeMode::setSafeMode(true);
                Serial.println("Safe mode activated");
            } else if(server.arg(i).equals("false")) {
                SafeMode::setSafeMode(false);
                Serial.println("Safe mode deactivated");
            }
        }

    sendResult("Safe Mode changed");
}

void RemoteControl::setHupe() {
    // get parameters
    for(int i = 0; i < server.args(); i++)
        if(server.argName(i).equals("value")) {
            if(server.arg(i).equals("start")) {
                // if(hupTask != NULL)
                //     vTaskDelete(hupTask);
                Speaker::startHupe();
                // xTaskCreate([](void*){long start = millis();for(;;){Speaker::signal();if(millis()-start>1000){hupTask=NULL;vTaskDelete(NULL);}}}, "Hup Task", 1024, NULL, 3, &hupTask);
                Serial.println("Hupe started");
            } else if(server.arg(i).equals("stop")) {
                // if(hupTask != NULL) {
                //     vTaskDelete(hupTask);
                //     hupTask = NULL;
                // }
                Speaker::stopHupe();
                Serial.println("Hupe stopped");
            }
        }
    sendResult("Hupe changed");
}

void RemoteControl::setLightMode() {
    // get parameters
    for(int i = 0; i < server.args(); i++)
        if(server.argName(i).equals("mode")) {
            // set light mode
            if(server.arg(i).equals("0")) {
                LightManager::setLightMode(OFF);
                Serial.println("Light Mode: off");
            } else if(server.arg(i).equals("1")) {
                LightManager::setLightMode(AUTO);
                Serial.println("Light Mode: auto");
            } else if(server.arg(i).equals("2")) {
                LightManager::setLightMode(ON);
                Serial.println("Light Mode: on");
            }
        }
    sendResult("light mode changed");
}

void RemoteControl::setLaser() {
    // get parameters
    for(int i = 0; i < server.args(); i++)
        if(server.argName(i).equals("mode")) {
            // set laser mode
            if(server.arg(i).equals("true")) {
                LightManager::setLaser(true);
                Serial.println("Laser activated");
            } else if(server.arg(i).equals("false")) {
                LightManager::setLaser(false);
                Serial.println("Laser deactivated");
            }
        }
    sendResult("laser changed");
}

void RemoteControl::sendResult(String content) {
    // if first send request
    if(lastReqeust==UINT32_MAX) {
        // dectivate blinking
        LightManager::setBrakeLights(false);
        lastReqeust = millis();
    }
    // send result to app
    server.send(200, "text/html", content);  
}