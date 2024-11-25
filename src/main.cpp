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
char old_hhmm[6];
char hhmm[6];

// 天气相关变量
RealTimeWeather rt_weather;

int loop_counter = 0;


void setup() {
    Serial.begin(9600);
    wifiConnect();
    serverInit();
    displayInit();
    updateTime();
    displayUpdateAll();
}

void loop() {
    wifiCheck();
    updateTime();
    if(old_hhmm[4]!=hhmm[4]) displayUpdateTime();
    loop_counter++;
    for(int i = 0;i<6;i++){
        old_hhmm[i] = hhmm[i];
    }
//     Serial.print("wifi_status: ");
//     Serial.println(wifi_status);
//     Serial.print("old_hhmm: ");
//     Serial.println(old_hhmm);
//     Serial.print("hhmm: ");
//     Serial.println(hhmm);
//     Serial.println(loop_counter);
}