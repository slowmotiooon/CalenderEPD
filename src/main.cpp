#include <Arduino.h>
#include <WifiConnection.h>
#include <WebServer.h>
#include <WebRequests.h>
#include <Commons.h>

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
}

void loop() {
    wifiCheck();
    updateTime();
    char timeStr[9];
    snprintf(timeStr, sizeof(timeStr), "%02d:%02d:%02d", time_info.tm_hour, time_info.tm_min, time_info.tm_sec);
    Serial.println(timeStr);
    getWeather();
    delay(10000); // Add a delay to avoid flooding the serial output
}