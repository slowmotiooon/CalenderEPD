#pragma once

struct ElectricData{
    double freeVol = 0;
    double vol = 0;
    double i = 0;
    double u = 0;
    double w = 0;
};

struct RealTimeWeather{
    String city = "";
    String weather = "";
    String temperature = "";
    String winddirection = "";
    String windpower = "";
    String humidity = "";
};