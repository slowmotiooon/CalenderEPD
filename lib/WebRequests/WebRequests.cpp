#include <WebRequests.h>
#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Commons.h>

HTTPClient http;

extern ElectricData e_data;
extern bool wifi_status;
extern struct tm time_info;

const char* ntpServer = "ntp.ntsc.ac.cn";

void updateTime() {
    if (!wifi_status) {
        time_t now = time(nullptr);
        time_info   = *localtime(&now);
        return;
    }

    configTime(8 * 3600, 0, ntpServer);
    // Serial.println("update time");
    if (!getLocalTime(&time_info, 1500)) return;
}

void getElectricityUsage(){
    http.begin("https://app.bupt.edu.cn/buptdf/wap/default/search");
    http.addHeader("Accept", "application/json, text/javascript, */*; q=0.01");
    http.addHeader("Accept-Language", "zh-CN,zh;q=0.9");
    http.addHeader("Connection", "keep-alive");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
    http.addHeader("Cookie", "eai-sess=2k84brh8at3j7pcaokkop4njb3; UUkey=fe7c670030f2781133e55a7af8f2365a; UUkey=2f925bce0e644a8f0ad75c38695956a1; eai-sess=suv1mtl2u5vq7k0ltpc4ipqs06");
    http.addHeader("Origin", "https://app.bupt.edu.cn");
    http.addHeader("Referer", "https://app.bupt.edu.cn/buptdf/wap/default/chong");
    http.addHeader("Sec-Fetch-Dest", "empty");
    http.addHeader("Sec-Fetch-Mode", "cors");
    http.addHeader("Sec-Fetch-Site", "same-origin");
    http.addHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/132.0.0.0 Safari/537.36");
    http.addHeader("X-Requested-With", "XMLHttpRequest");
    http.addHeader("sec-ch-ua", "\"Not A(Brand\";v=\"8\", \"Chromium\";v=\"132\", \"Google Chrome\";v=\"132\"");
    http.addHeader("sec-ch-ua-mobile", "?0");
    http.addHeader("sec-ch-ua-platform", "\"Windows\"");

    String payload = "partmentId=ac3a79c0e22e495096577d3aef1f5532&floorId=5&dromNumber=6-539&areaid=1";
    int code = http.POST(payload);
    Serial.println("Request sent");
    Serial.println(code);
    String result = http.getString();
    if(code==200){
        JsonDocument doc;
        deserializeJson(doc, result);
        e_data.u = doc["d"]["data"]["vTotal"];
        e_data.i = doc["d"]["data"]["iTotal"];
        e_data.w=e_data.u*e_data.i;
        e_data.vol = doc["d"]["data"]["surplus"];
        e_data.freeVol = doc["d"]["data"]["free"];
        e_data.vol+=e_data.freeVol;
    }
    Serial.print("Voltage (U): ");
    Serial.println(e_data.u);
    Serial.print("Current (I): ");
    Serial.println(e_data.i);
    Serial.print("Power (W): ");
    Serial.println(e_data.w);
    Serial.print("Total Volume: ");
    Serial.println(e_data.vol);
    Serial.print("Free Volume: ");
    Serial.println(e_data.freeVol);
}
