#include <Arduino.h>
#include <ArduinoJson.h>
#include <Commons.h>
#include <HTTPClient.h>
#include <WebRequests.h>

HTTPClient  http;
const char* ntpServer        = "ntp.aliyun.com";
const char* realtime_weather = "https://restapi.amap.com/v3/weather/"
                               "weatherInfo?key=82a7c1f0309a05a4f5074a9a5c42a248&city=110108";
const char* forecast_weather =
    "https://restapi.amap.com/v3/weather/"
    "weatherInfo?key=82a7c1f0309a05a4f5074a9a5c42a248&city=110108&extensions=all";

extern ElectricData    e_data;
extern RealTimeWeather rt_weather;
extern Forecast forecast;

extern bool      wifi_status;
extern struct tm time_info;
extern String    current_time;


void updateTime() {
    if (!wifi_status) {
        time_t now = time(nullptr);
        time_info  = *localtime(&now);
        return;
    }
    configTime(8 * 3600, 0, ntpServer);
    if (!getLocalTime(&time_info, 3000)) { return; }
}

void getElectricityUsage() {
    http.begin("https://app.bupt.edu.cn/buptdf/wap/default/search");
    http.addHeader("Accept", "application/json, text/javascript, */*; q=0.01");
    http.addHeader("Accept-Language", "zh-CN,zh;q=0.9");
    http.addHeader("Connection", "keep-alive");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
    http.addHeader("Cookie",
                   "eai-sess=2k84brh8at3j7pcaokkop4njb3; UUkey=fe7c670030f2781133e55a7af8f2365a; "
                   "UUkey=2f925bce0e644a8f0ad75c38695956a1; eai-sess=suv1mtl2u5vq7k0ltpc4ipqs06");
    http.addHeader("Origin", "https://app.bupt.edu.cn");
    http.addHeader("Referer", "https://app.bupt.edu.cn/buptdf/wap/default/chong");
    http.addHeader("Sec-Fetch-Dest", "empty");
    http.addHeader("Sec-Fetch-Mode", "cors");
    http.addHeader("Sec-Fetch-Site", "same-origin");
    http.addHeader("User-Agent",
                   "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like "
                   "Gecko) Chrome/132.0.0.0 Safari/537.36");
    http.addHeader("X-Requested-With", "XMLHttpRequest");
    http.addHeader("sec-ch-ua",
                   "\"Not A(Brand\";v=\"8\", \"Chromium\";v=\"132\", \"Google Chrome\";v=\"132\"");
    http.addHeader("sec-ch-ua-mobile", "?0");
    http.addHeader("sec-ch-ua-platform", "\"Windows\"");

    String payload =
        "partmentId=ac3a79c0e22e495096577d3aef1f5532&floorId=5&dromNumber=6-539&areaid=1";
    int code = http.POST(payload);
    Serial.println("Request sent");
    Serial.println(code);
    String result = http.getString();
    if (code == 200) {
        JsonDocument doc;
        deserializeJson(doc, result);
        e_data.u       = doc["d"]["data"]["vTotal"];
        e_data.i       = doc["d"]["data"]["iTotal"];
        e_data.w       = e_data.u * e_data.i;
        e_data.vol     = doc["d"]["data"]["surplus"];
        e_data.freeVol = doc["d"]["data"]["free"];
        e_data.vol += e_data.freeVol;
    }
}

void getWeather() {
    http.begin(realtime_weather);
    int code = http.GET();
    Serial.println("Request sent");
    Serial.println(code);
    String result = http.getString();
    if (code == 200) {
        JsonDocument doc;
        deserializeJson(doc, result);
        const char* r_city          = doc["lives"][0]["city"];
        const char* r_weather       = doc["lives"][0]["weather"];
        const char* r_temperature   = doc["lives"][0]["temperature"];
        const char* r_winddirection = doc["lives"][0]["winddirection"];
        const char* r_windpower     = doc["lives"][0]["windpower"];
        const char* r_humidity      = doc["lives"][0]["humidity"];
        rt_weather.city             = r_city;
        rt_weather.weather          = r_weather;
        rt_weather.temperature      = r_temperature;
        rt_weather.winddirection    = r_winddirection;
        rt_weather.windpower        = r_windpower;
        rt_weather.humidity         = r_humidity;
    }
    Serial.print("City: ");
    Serial.println(rt_weather.city);
    Serial.print("Weather: ");
    Serial.println(rt_weather.weather);
    Serial.print("Temperature: ");
    Serial.println(rt_weather.temperature);
    Serial.print("Wind Direction: ");
    Serial.println(rt_weather.winddirection);
    Serial.print("Wind Power: ");
    Serial.println(rt_weather.windpower);
    Serial.print("Humidity: ");
    Serial.println(rt_weather.humidity);
}

void getForecast(){
    http.begin(forecast_weather);
    int code = http.GET();
    Serial.println("Request sent");
    Serial.println(code);
    String result = http.getString();
    if (code == 200) {
        JsonDocument doc;
        deserializeJson(doc, result);
        const char* f_tmr_dweather = doc["forecasts"][0]["casts"][1]["dayweather"];
        const char* f_tmr_nweather = doc["forecasts"][0]["casts"][1]["nightweather"];
        const char* f_tmr_dtemp = doc["forecasts"][0]["casts"][1]["daytemp"];
        const char* f_tmr_ntemp = doc["forecasts"][0]["casts"][1]["nighttemp"];
        const char* f_aft_dweather = doc["forecasts"][0]["casts"][2]["dayweather"];
        const char* f_aft_nweather = doc["forecasts"][0]["casts"][2]["nightweather"];
        const char* f_aft_dtemp = doc["forecasts"][0]["casts"][2]["daytemp"];
        const char* f_aft_ntemp = doc["forecasts"][0]["casts"][2]["nighttemp"];
        forecast.tmr_dweather = f_tmr_dweather;
        forecast.tmr_nweather = f_tmr_nweather;
        forecast.tmr_dtemp = f_tmr_dtemp;
        forecast.tmr_ntemp = f_tmr_ntemp;
        forecast.aft_dweather = f_aft_dweather;
        forecast.aft_nweather = f_aft_nweather;
        forecast.aft_dtemp = f_aft_dtemp;
        forecast.aft_ntemp = f_aft_ntemp;
    }
    Serial.print("Tomorrow Day Weather: ");
    Serial.println(forecast.tmr_dweather);
    Serial.print("Tomorrow Night Weather: ");
    Serial.println(forecast.tmr_nweather);
    Serial.print("Tomorrow Day Temperature: ");
    Serial.println(forecast.tmr_dtemp);
    Serial.print("Tomorrow Night Temperature: ");
    Serial.println(forecast.tmr_ntemp);
    Serial.print("Day After Tomorrow Day Weather: ");
    Serial.println(forecast.aft_dweather);
    Serial.print("Day After Tomorrow Night Weather: ");
    Serial.println(forecast.aft_nweather);
    Serial.print("Day After Tomorrow Day Temperature: ");
    Serial.println(forecast.aft_dtemp);
    Serial.print("Day After Tomorrow Night Temperature: ");
    Serial.println(forecast.aft_ntemp);

}