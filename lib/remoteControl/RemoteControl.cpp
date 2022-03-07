#include "RemoteControl.h"

Mode RemoteControl::mode = NONE;
const char* RemoteControl::ssid[4] = {"ESP-Bot", "JCBS-Schüler", "NetFrame", "ESP32"};
const char* RemoteControl::password[4] = {"12345678", "S1,16DismdEn;deieKG!", "87934hzft9oeu4389nv8o437893hf978", "12345678"};
const char* RemoteControl::hostname = "ESP32_Bot";
ESP32WebServer RemoteControl::server(80);
unsigned long RemoteControl::lastReqeust = UINT32_MAX;
int RemoteControl::speed = 0;
bool RemoteControl::correction = false;
xTaskHandle hupTask = NULL;

bool testState = false;

void RemoteControl::setup() {
    //WIFI connection
    WiFi.mode(WIFI_AP_STA);
    WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
    WiFi.setHostname(hostname);
    for(int i = 1; i < sizeOf(ssid); i++) {
        WiFi.begin(ssid[i%sizeOf(ssid)], password[i%sizeOf(password)]);
        for(int j = 0; j < 5; j++) {
            Serial.println("Connecting to "+String(ssid[i])+"...");
            delay(1000);
            if(WiFi.status() == WL_CONNECTED)
                break;
        }
        if(WiFi.status() == WL_CONNECTED)
            break;
    }
    if(WiFi.status() == WL_CONNECTED)
        Serial.println(WiFi.localIP());
    else {
        WiFi.softAP(ssid[0], password[0]);
        Serial.println(WiFi.softAPIP());
    }

    //Server
    server.on("/direction", setDirection);
    server.on("/mode", setMode);
    server.on("/hupe", setHupe);
    server.on("/lightMode", setLightMode);
    server.on("/safeMode", setSafeMode);
    server.on("/laser", setLaser);
        
    server.begin();
    Serial.println("Server starteted");
    xTaskCreate(RemoteControl::loop, "remote", 4*1024, NULL, 5, NULL);
}
 
void RemoteControl::loop(void*) { 
    unsigned long lastBlink = 0; 
    bool blinkState = 0;
    for(;;) {
        if(lastReqeust == UINT32_MAX || (mode == REMOTE && millis()-lastReqeust > 1000)) {
            if(lastReqeust != UINT32_MAX) {
                Motor::setSpeeds(0);
                lastReqeust = UINT32_MAX;
            }
            if(millis()-lastBlink > 1000) {
                lastBlink = millis();
                blinkState = !blinkState;
                LightManager::setBremsLight(blinkState);
            }
        }
        server.handleClient();
        vTaskDelay(5);
    }
}

Mode RemoteControl::getMode() {
    return mode;
}
void RemoteControl::setDirection() {
    if(mode != REMOTE || correction) {
        sendResult("Direction not changed");
        return;
    }
    lastReqeust = millis();
    int angle = 0;
    int strength = 0;
    int speed = 0;
    for (int i = 0; i < server.args(); i++)
        if(server.argName(i).equals("angle")) {
            std::string s = server.arg(i).c_str();
            std::stringstream intValue(s);
            intValue >> angle;            
        } else if(server.argName(i).equals("strength")) {
            std::string s = server.arg(i).c_str();
            std::stringstream intValue(s);
            intValue >> strength;            
        } else if(server.argName(i).equals("speed")) {
            std::string s = server.arg(i).c_str();
            std::stringstream intValue(s);
            intValue >> speed;            
        }
    int a = (180-angle%180)*PI/180;
    int x = /*(angle==0?-1:1)*strength;*/cos(a)*strength;
    int y = sin(a)*strength;
    // Serial.print(speed);
    if(speed < 0)
        LightManager::setBremsLight(true);
    else
        LightManager::setBremsLight(false);
    if(speed != 0)
        speed = (speed<0?speed/abs(speed):1)*map(abs(speed), 0, 100, 50, 200);
    double _speed = 0;
    double _speed2 = 0;
    if(speed == 0) {
        if(abs(x)>20) {
            _speed = map(abs(x), 20, 100, 70, 100)/255.;
            _speed2 = -_speed;
        }
    } else {
         double v = 1-abs(cos(a)*strength/100.);
        _speed = speed/255.;
        if(abs(_speed) < 0.4)
            _speed = 0.;
        _speed2 = v*_speed;
        _speed2 = (_speed2<0?_speed2/abs(_speed2):1)*MAP(abs(_speed2), 0., abs(_speed), .3, abs(_speed));
        if(abs(_speed2) < 0.4)
            _speed2 = 0.;
    }
    // Serial.print(x/abs(x));
    // Serial.s;    
    if(x>0) {
        motor1.setSpeed(_speed2);
        motor2.setSpeed(_speed);
    } else {
        motor1.setSpeed(_speed);
        motor2.setSpeed(_speed2);
    }
    // Serial.print(" Motor 1: ");
    // Serial.print(motor1.getSpeed());
    // Serial.print(" Motor 2: ");
    // Serial.println(motor2.getSpeed());
    // Absenden eines JSONS mit einer Begrüßung und unseres gelesenen Textes.
    sendResult("Direction changed");
}

void RemoteControl::setSpeed() {
    // sendResult("Speed changed");
    for (int i = 0; i < server.args(); i++)
        if(server.argName(i).equals("strength")) {
            std::string s = server.arg(i).c_str();
            std::stringstream intValue(s);
            intValue >> speed;
            speed=map(speed, 0, 100, 100, 255);           
        }
    Serial.print("speed: ");
    Serial.println(speed);
    //Absenden eines JSONS mit einer Begrüßung und unseres gelesenen Textes.
    sendResult("Speed changed");
}

void RemoteControl::setMode() {
    for (int i = 0; i < server.args(); i++)
        if(server.argName(i).equals("mode")) {
            std::string s = server.arg(i).c_str();
            std::stringstream intValue(s);
            int value = 0;
            intValue >> value;
            mode = (Mode) value;
            Serial.println(mode);
            motor1.setSpeed(0);
            motor2.setSpeed(0);
            LightManager::setBremsLight(false);
        }
    //Absenden eines JSONS mit einer Begrüßung und unseres gelesenen Textes.
    sendResult("Mode set to "+String(mode)+"");
}

void RemoteControl::setSafeMode() {
    for(int i = 0; i < server.args(); i++)
        if(server.argName(i).equals("mode")) {
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
    for(int i = 0; i < server.args(); i++)
        if(server.argName(i).equals("mode")) {
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
    for(int i = 0; i < server.args(); i++)
        if(server.argName(i).equals("mode")) {
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

//Diese Funktion sendet eine Antwort an den Client.
void RemoteControl::sendResult(String content) {
    //200 ist die Antwort das alles OK ist,
    //text/html ist der MimeType
    //content ist unser Text
    if(lastReqeust==UINT32_MAX) {
        LightManager::setBremsLight(false);
        lastReqeust = millis();
    }
    server.send(200, "text/html", content);  
}