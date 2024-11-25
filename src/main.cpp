#include <Arduino.h>
#include <WifiConnection.h>
#include <WebServer.h>
#include <WebRequests.h>
#include <Commons.h>
#include <EpdDisplay.h>
#include <UpdateInfo.h>


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
String current_time;

// 天气相关变量
RealTimeWeather rt_weather;

int loop_counter = 0;


void setup() {
    Serial.begin(115200);
    wifiConnect();
    serverInit();
    displayInit();
    updateTime();
    displayUpdateAll();
    delay(5000);
}

void loop() {
    wifiCheck();
    updateTime();
    if(needToUpdateTime()) displayUpdateTime();
    current_time = asctime(&time_info);
    loop_counter++;
    Serial.print("wifi_status: ");
    Serial.println(wifi_status);
    Serial.print("time_info_hour: ");
    Serial.println(time_info.tm_hour);
    Serial.print("time_info_min: ");
    Serial.println(time_info.tm_min);
    Serial.print("time_info_sec: ");
    Serial.println(time_info.tm_sec);
    Serial.println("time_info: ");
    Serial.println(asctime(&time_info));
    Serial.println(loop_counter);
}