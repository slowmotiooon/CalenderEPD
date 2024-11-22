#include <WebServer.h>
#include <Arduino.h>
#include <ESPAsyncWebServer.h>


AsyncWebServer server(80);

void serverInit(){
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "Hello, world");
    });
    server.begin();
}

