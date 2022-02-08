#include "RemoteControl.h"

const char* RemoteControl::ssid = "NetFrame";  //replace
const char* RemoteControl::password =  "87934hzft9oeu4389nv8o437893hf978"; //replace
ESP32WebServer RemoteControl::server(80);

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
    //Wenn der Server angewiesen wird das Servlet mit der Bezeichnung "greeting" bereitzustellen
    //so wird die Funktion "callGreeting" aufgerufen.
    server.on("/greeting", callGreeting);
        
    server.begin(); // Starten des Servers.
    Serial.println("Server gestartet"); //Ausgabe auf der Seriellen Schnittstelle das der Server gestartet wurde.
    
    // Ausgabe der IP Adresse 
    Serial.print("Adresse : http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");
}
 
void RemoteControl::loop() {  
    //alle Anfragen der Clients verarbeiten.
    server.handleClient();
    //Hier wird keine Pause eingelegt da dieses sonst die Verarbeitung stören würde.
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
    sendResult("{\"msg\": \"Hello from ESP8266!- "+text+"\"}");
 }

//Diese Funktion sendet eine Antwort an den Client.
void RemoteControl::sendResult(String content) {
    //200 ist die Antwort das alles OK ist,
    //text/html ist der MimeType
    //content ist unser Text
    server.send(200, "text/html", content);  
}