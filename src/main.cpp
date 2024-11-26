#include <Arduino.h>
#include <Commons.h>
#include <EpdDisplay.h>
#include <Temperature.h>
#include <UpdateInfo.h>
#include <WebRequests.h>
#include <WebServer.h>
#include <WifiConnection.h>


// Wi-Fi相关变量
String ssid        = "X-WRT_FFA3";
String password    = "88888888";
String ip          = "";
String mac         = "";
bool   wifi_status = false;

// 电量查询相关变量
ElectricData e_data;
ElectricData old_e_data;

// 时间相关变量
struct tm time_info;
struct tm old_time_info;

// 天气相关变量
RealTimeWeather rt_weather;
Forecast forecast;

// 温湿度相关变量
Temperature t_data;
Temperature old_t_data;

int loop_counter = 0;
bool partial = true;


void setup() {
    Serial.begin(115200);
    wifiConnect();
    serverInit();
    tempInit();
    getElectricityUsage();
    displayInit();
    updateTime();
    displayUpdateAll();
    delay(5000);
}

void loop() {
    wifiCheck();
    updateTime();
    updateTemperature();
    if (needToUpdateElec()) getElectricityUsage();
    if (needToUpdateWeather()) getWeather();
    if (needToUpdateForecast()) getForecast();
    if (needToUpdateInfo()) displayUpdateAll(partial);
    old_time_info = time_info;
    old_e_data    = e_data;
    old_t_data    = t_data;
    partial = true;
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