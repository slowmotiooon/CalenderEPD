#include <Arduino.h>
#include <WifiConnection.h>
#include <WebServer.h>
#include <WebRequests.h>
#include <Commons.h>
#include <EpdDisplay.h>


// Wi-Fi相关变量
String ssid = "X-WRT_FFA3";
String password = "88888888";
String ip = "";
String mac = "";
bool wifi_status = false;

// 电量查询相关变量
ElectricData e_data;

// 时间相关变量
struct tm time_info;

// 天气相关变量
RealTimeWeather rt_weather;


void setup() {
    Serial.begin(9600);
    wifiConnect();
    serverInit();
    displayInit();
    displayBackground();
}

void loop() {
    wifiCheck();
    updateTime();
    delay(5000);
}