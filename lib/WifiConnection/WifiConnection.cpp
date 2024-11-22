#include <WifiConnection.h>
#include <Arduino.h>
#include <WiFi.h>

extern String ssid;
extern String password;
extern String ip;
extern String mac;
extern bool wifi_status;

void wifiConnect(){
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(),password.c_str());
    mac = WiFi.macAddress();
}

void wifiCheck(){
    if(WiFi.status() == WL_CONNECTED){
        wifi_status = true;
        if(ip.equals(WiFi.localIP().toString())) ip = WiFi.localIP().toString();
    }else{
        wifi_status = false;
    }
}
