#include <Arduino.h>
#include <WifiConnection.h>

// Wi-Fi相关变量
String ssid = "X-WRT_FFA3";
String password = "88888888";
String ip = "";
String mac = "";
bool wifi_status = false;


void setup() {
    Serial.begin(9600);
    wifiConnect();

}

void loop() {
    wifiCheck();
    if(wifi_status){
        
    } else {
        Serial.println("Wi-Fi disconnected");
    }
}