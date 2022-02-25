#include "RemoteControl.h"

Mode RemoteControl::mode = NONE;
const char* RemoteControl::ssid[4] = {"ESP-Bot", "JCBS-Schüler", "ESP32", "NetFrame"};
const char* RemoteControl::password[4] = {"12345678", "K1,14DWwFuwuu.", "12345678", "87934hzft9oeu4389nv8o437893hf978"};
ESP32WebServer RemoteControl::server(80);
int RemoteControl::speed = 0;
bool RemoteControl::correction = false;

bool testState = false;

void RemoteControl::setup() {
    //WIFI connection
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
    //Wenn der Server angewiesen wird das Servlet mit der Bezeichnung "greeting" bereitzustellen
    //so wird die Funktion "callGreeting" aufgerufen.
    server.on("/greeting", callGreeting);
    server.on("/direction", setDirection);
    server.on("/speed", setSpeed);
    server.on("/mode", setMode);
        
    server.begin(); // Starten des Servers.
    Serial.println("Server gestartet"); //Ausgabe auf der Seriellen Schnittstelle das der Server gestartet wurde.

    xTaskCreate(RemoteControl::loop, "remote", 4*1024, NULL, 5, NULL);
}
 
void RemoteControl::loop(void*) {  
    for(;;) {
        //alle Anfragen der Clients verarbeiten.
        server.handleClient();
        //Hier wird keine Pause eingelegt da dieses sonst die Verarbeitung stören würde.
    }
}

Mode RemoteControl::getMode() {
    return mode;
}

void RemoteControl::setDirection() {
    if(mode != REMOTE || correction) {
        sendResult("{\"msg\": \"Direction not changed\"}");
        return;
    }
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
    if(speed != 0)
        speed = (speed<0?speed/abs(speed):1)*map(abs(speed), 0, 100, 60, 255);
    double _speed = 0;
    double _speed2 = 0;
    if(speed == 0) {
        if(abs(x)>20) {
            _speed = map(abs(x), 20, 100, 100, 150)/255.;
            _speed2 = -_speed;
        }
    } else {
         double v = 1-abs(cos(a)*strength/100.);
        _speed = speed/255.;
        if(abs(_speed) < 0.4)
            _speed = 0.;
        _speed2 = v*_speed;
        _speed2 = (_speed2<0?_speed2/abs(_speed2):1)*MAP(abs(_speed2), 0., abs(_speed), .39, abs(_speed));
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
    sendResult("{\"msg\": \"Direction changed\"}");
}

void RemoteControl::setSpeed() {
    // sendResult("{\"msg\": \"Speed changed\"}");
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
    sendResult("{\"msg\": \"Speed changed\"}");
}

void RemoteControl::setMode() {
    for (int i = 0; i < server.args(); i++)
        if(server.argName(i).equals("mode")) {
            std::string s = server.arg(0).c_str();
            std::stringstream intValue(s);
            int value = 0;
            intValue >> value;
            mode = (Mode) value;
            Serial.println(mode);
        }
    //Absenden eines JSONS mit einer Begrüßung und unseres gelesenen Textes.
    sendResult("{\"msg\": \"Mode set to "+String(mode)+"\"}");
}

void RemoteControl::callGreeting() {
    //Eine Variable zum speichern des gelesenen Wertes. 
    String text = "-undefined-";
    //Über alle möglichen Parameter iterieren.
    for (int i = 0; i < server.args(); i++) {

        //Zuweisen der Schlüssel/Wertepaare
        String parameterName = server.argName(i);
        String parameterValue = server.arg(i);
        //Wenn der Parametername gleich "text" ist dann...
        if(parameterName == "text"){
            //zuweisen des Wertes zu unserer Variable
            text = parameterValue;      
        } 
    }
    //Absenden eines JSONS mit einer Begrüßung und unseres gelesenen Textes.
    sendResult("{\"msg\": \"Hello from ESP32!- "+text+"\"}");
 }

//Diese Funktion sendet eine Antwort an den Client.
void RemoteControl::sendResult(String content) {
    //200 ist die Antwort das alles OK ist,
    //text/html ist der MimeType
    //content ist unser Text
    server.send(200, "text/html", content);  
}