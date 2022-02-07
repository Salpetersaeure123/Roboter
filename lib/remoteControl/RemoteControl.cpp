#include "RemoteControl.h"

const char* RemoteControl::ssid = "NetFrame";  //replace
const char* RemoteControl::password =  "87934hzft9oeu4389nv8o437893hf978"; //replace
AsyncWebServer RemoteControl::server(80);

bool testState = false;

void RemoteControl::setup() {
    //WIFI connection
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }
    Serial.println(WiFi.localIP());
 
    //Server
    server.on("/hello", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "Hello World");
    });

    server.on("/relay/off", HTTP_GET   , [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "ok");
        testState = false;
        Serial.println(testState);
    });
    server.on("/relay/on", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain","ok");
        testState = true;
        Serial.println(testState);
    });
    
    server.on("/relay/toggle", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain","ok");
        testState = !testState;
        Serial.println(testState);
    });
    
    server.on("/relay", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", String(testState));
    });
  
    server.begin();
}