#pragma once

#include <Arduino.h>

// 引脚定义
#define EPD_CS 1
#define EPD_DC 2
#define EPD_RST 3
#define EPD_BUSY 4
#define EPD_CLK 5
#define EPD_MOSI 6
#define EPD_MISO 14 // Not used


// 结构体定义
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

struct PartialWindow{
    int x;
    int y;
    int w;
    int h;
};

// 字体定义:
// #define CLOCK_FONT u8g2_font_inb46_mn
#define CLOCK_FONT u8g2_font_logisoso54_tn
#define DATE_FONT u8g2_font_wqy14_t_gb2312
#define ELECTRICITY_FONT u8g2_font_wqy16_t_gb2312
#define TEMPERATURE_FONT u8g2_font_wqy16_t_gb2312
#define TEMPERATURE_ICON u8g2_font_unifont_t_weather
#define WEATHER_ICON u8g2_font_open_iconic_weather_4x
#define CITY_FONT u8g2_font_wqy16_t_gb2312
#define FORCAST_FONT u8g2_font_wqy12_t_gb2312
#define CALENDAR_NUM_FONT u8g2_font_logisoso26_tn
#define CALENDAR_DAY_FONT u8g2_font_wqy12_t_gb2312
#define CALENDAR_WEEK_FONT u8g2_font_wqy12_t_gb2312
#define CALENDER_EVENT_FONT u8g2_font_5x8_tn


// 模块分块刷新窗口声明
extern const PartialWindow time_window;
extern const PartialWindow date_window;
extern const PartialWindow elec_window;
extern const PartialWindow weather_window;
extern const PartialWindow pic_window;
extern const PartialWindow cal_window[5][7];
extern const PartialWindow whole_screen;

