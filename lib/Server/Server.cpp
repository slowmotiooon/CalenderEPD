#include <Arduino.h>
#include <WiFiServer.h>

WiFiServer server(80);

void serverInit(){
    server.begin();
}

