#include "GxEPD2_BW.h"
#include "epd/GxEPD2_583_T8.h"
#include <Arduino.h>
#include <Commons.h>
#include <EpdDisplay.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <U8g2_for_Adafruit_GFX.h>

#include <flash_on_20px.c>
#include <humidity_mid.c>
#include <i_20px.c>
#include <thermostat.c>
#include <v_20px.c>
#include <w_20px.c>

#include <air.c>
#include <bolt.c>
#include <cloud.c>
#include <cloudy_snowing.c>
#include <foggy.c>
#include <mist.c>
#include <partly_cloudy_day.c>
#include <question_mark.c>
#include <rainy.c>
#include <rainy_heavy.c>
#include <rainy_light.c>
#include <rainy_snow.c>
#include <snowing.c>
#include <snowing_heavy.c>
#include <storm.c>
#include <sunny.c>

#include <p1.c>
#include <p10.c>
#include <p11.c>
#include <p12.c>
#include <p13.c>
#include <p2.c>
#include <p3.c>
#include <p4.c>
#include <p5.c>
#include <p6.c>
#include <p7.c>
#include <p8.c>
#include <p9.c>

SPISettings                                         epd_spi_settings;
SPIClass                                            epd_spi(HSPI);
GxEPD2_BW<GxEPD2_583_T8, GxEPD2_583_T8::HEIGHT / 2> display(
    GxEPD2_583_T8(/*CS*/ EPD_CS, /*DC*/ EPD_DC, /*RST*/ EPD_RST, /*BUSY*/ EPD_BUSY));
U8G2_FOR_ADAFRUIT_GFX u8g2_fonts;

const String week[] = { "星期日", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六" };
const unsigned char* pic[]     = { pic_p1, pic_p2, pic_p3,  pic_p4,  pic_p5,  pic_p6, pic_p7,
                                   pic_p8, pic_p9, pic_p10, pic_p11, pic_p12, pic_p13 };
int                  pic_index = 4;

extern tm              time_info;
extern ElectricData    e_data;
extern Temperature     t_data;
extern RealTimeWeather rt_weather;
extern Forecast        forecast;

void displayInit() {
    epd_spi.begin(EPD_CLK, EPD_MISO, EPD_MOSI, EPD_CS);
    epd_spi_settings = SPISettings();
    display.init(115200, true, 2, false, epd_spi, epd_spi_settings);
    display.setRotation(0);
    u8g2_fonts.begin(display);
    u8g2_fonts.setFontMode(1);
    u8g2_fonts.setFontDirection(0);
    u8g2_fonts.setForegroundColor(GxEPD_BLACK);
    u8g2_fonts.setBackgroundColor(GxEPD_WHITE);
}

void displayTimeTest() {
    display.setPartialWindow(0, 0, 128, 24);
    display.setTextColor(GxEPD_BLACK);
    display.setFont(&FreeMonoBold9pt7b);
    display.firstPage();
    do { display.setCursor(0, 20); } while (display.nextPage());
}

void updatePic() {
    pic_index = (pic_index + 1) % 13;
}

const unsigned char* getWeatherIcon() {
    String w = rt_weather.weather;
    if (w.equals("晴")) return icon_sunny;
    if (w.endsWith("云")) return icon_partly_cloudy_day;
    if (w.equals("阴")) return icon_cloud;
    if (w.endsWith("风") || w.endsWith("风暴")) {
        if (w.endsWith("风暴") || w.equals("狂爆风") || w.equals("飓风") || w.equals("龙卷风"))
            return icon_storm;
        else
            return icon_air;
    }
    if (w.endsWith("霾")) return icon_mist;
    if (w.startsWith("雷")) return icon_bolt;
    if (w.endsWith("雨")) {
        if (w.equals("强雷阵雨")) return icon_bolt;
        if (w.startsWith("小雨") || w.startsWith("毛")) return icon_rainy;
        if (w.endsWith("暴雨") || w.equals("大雨") || w.equals("强阵雨") || w.equals("极端降雨"))
            return icon_rainy_heavy;
        else
            return icon_rainy_light;
    }
    if (w.equals("雨雪天气") || w.endsWith("雨夹雪")) return icon_rainy_snow;
    if (w.endsWith("雪")) {
        if (w.startsWith("小雪") || w.equals("阵雪")) return icon_cloudy_snowing;
        if (w.endsWith("暴雪") || w.equals("大雪"))
            return icon_snowing_heavy;
        else
            return icon_snowing;
    }
    if (w.endsWith("沙尘暴") || w.endsWith("浮尘") || w.endsWith("扬沙")) return icon_mist;
    if (w.endsWith("雾")) return icon_foggy;
    return icon_question_mark;
}

void displayBackground() {
    display.setFullWindow();
    display.firstPage();
    do {
        display.fillScreen(GxEPD_WHITE);
        display.drawRect(16, 16, 617, 449, GxEPD_BLACK);    // 边框
        display.drawLine(16, 120, 632, 120, GxEPD_BLACK);   // 分割线

        display.drawLine(16, 134, 632, 134, GxEPD_BLACK);
        display.drawLine(16, 200, 632, 200, GxEPD_BLACK);
        display.drawLine(16, 266, 632, 266, GxEPD_BLACK);
        display.drawLine(16, 332, 632, 332, GxEPD_BLACK);
        display.drawLine(16, 398, 632, 398, GxEPD_BLACK);

        display.drawLine(104, 120, 104, 464, GxEPD_BLACK);
        display.drawLine(192, 120, 192, 464, GxEPD_BLACK);
        display.drawLine(280, 120, 280, 464, GxEPD_BLACK);
        display.drawLine(368, 120, 368, 464, GxEPD_BLACK);
        display.drawLine(456, 120, 456, 464, GxEPD_BLACK);
        display.drawLine(544, 120, 544, 464, GxEPD_BLACK);

        display.drawRect(24, 24, 160, 64, GxEPD_BLACK);   // 时间
        u8g2_fonts.setFont(CLOCK_FONT);                   // 时间字体
        u8g2_fonts.drawUTF8(24, 86, "22:07");
        display.drawRect(24, 92, 160, 20, GxEPD_BLACK);   // 日期
        u8g2_fonts.setFont(DATE_FONT);                    // 日期字体
        u8g2_fonts.drawUTF8(25, 110, "2021年06月01日 星期二");

        display.drawRect(192, 24, 168, 88, GxEPD_BLACK);   // 电量和温湿度
        display.drawBitmap(193, 27, icon_thermostat, 20, 20, GxEPD_BLACK, GxEPD_WHITE);
        display.drawBitmap(193, 49, icon_v_20px, 20, 20, GxEPD_BLACK, GxEPD_WHITE);
        display.drawBitmap(193, 71, icon_w_20px, 20, 20, GxEPD_BLACK, GxEPD_WHITE);
        display.drawBitmap(273, 27, icon_humidity_mid, 20, 20, GxEPD_BLACK, GxEPD_WHITE);
        display.drawBitmap(273, 49, icon_i_20px, 20, 20, GxEPD_BLACK, GxEPD_WHITE);
        display.drawBitmap(273, 71, icon_flash_on_20px, 20, 20, GxEPD_BLACK, GxEPD_WHITE);
        u8g2_fonts.setFont(TEMPERATURE_FONT);   // 温湿度字体
        u8g2_fonts.drawUTF8(214, 44, "25℃");
        u8g2_fonts.drawUTF8(294, 44, "50%");
        u8g2_fonts.drawUTF8(214, 66, "220.0V");
        u8g2_fonts.drawUTF8(294, 66, "10.0A");
        u8g2_fonts.drawUTF8(214, 88, "2200.0W");
        u8g2_fonts.drawUTF8(294, 88, "127.45度");
        u8g2_fonts.drawUTF8(194, 110, "第48周, 教学周第14周");

        display.drawRect(368, 24, 160, 88, GxEPD_BLACK);   // 天气
        display.drawRect(368, 24, 40, 40, GxEPD_BLACK);    // 天气图标
        display.drawBitmap(369, 25, icon_air, 40, 40, GxEPD_BLACK, GxEPD_WHITE);
        u8g2_fonts.setFont(CITY_FONT);   // 城市字体
        u8g2_fonts.drawUTF8(409, 40, "海淀区");
        u8g2_fonts.drawUTF8(409, 60, "晴 25℃");
        u8g2_fonts.setFont(FORCAST_FONT);   // 预报字体
        u8g2_fonts.drawUTF8(371, 77, "西北风 5级");
        u8g2_fonts.drawUTF8(371, 92, "明天 中到大雪 -13/-11℃");
        u8g2_fonts.drawUTF8(371, 107, "后天 小雨 13/17℃");
        display.drawBitmap(536, 24, pic_p1, 88, 88, GxEPD_BLACK, GxEPD_WHITE);   // 图像

        u8g2_fonts.setFont(CALENDAR_WEEK_FONT);   // 日历星期字体
        u8g2_fonts.drawUTF8(48, 132, "周日");
        u8g2_fonts.drawUTF8(136, 132, "周一");
        u8g2_fonts.drawUTF8(224, 132, "周二");
        u8g2_fonts.drawUTF8(312, 132, "周三");
        u8g2_fonts.drawUTF8(400, 132, "周四");
        u8g2_fonts.drawUTF8(488, 132, "周五");
        u8g2_fonts.drawUTF8(576, 132, "周六");


        display.drawRect(24, 142, 72, 50, GxEPD_BLACK);   // 日历项边框
        display.drawRect(24, 142, 40, 50, GxEPD_BLACK);
        u8g2_fonts.setFont(CALENDAR_NUM_FONT);   // 日历字体
        u8g2_fonts.drawUTF8(25, 172, "27");
        u8g2_fonts.setFont(CALENDAR_DAY_FONT);   // 日历字体
        u8g2_fonts.drawUTF8(25, 186, "中秋节");
        u8g2_fonts.setFont(CALENDER_EVENT_FONT);   // 日历字体
        u8g2_fonts.drawUTF8(68, 150, "10:00");
        u8g2_fonts.drawUTF8(68, 158, "11:25");
        u8g2_fonts.drawUTF8(68, 166, "13:00");
        u8g2_fonts.drawUTF8(68, 174, "14:40");
        u8g2_fonts.drawUTF8(68, 182, "15:35");
        u8g2_fonts.drawUTF8(68, 190, "16:25");


    } while (display.nextPage());
}

void displayUpdateAll(bool partial) {
    char current_time[6];
    sprintf(current_time, "%02d:%02d", time_info.tm_hour, time_info.tm_min);

    String year  = String(time_info.tm_year + 1900);
    String month = String(time_info.tm_mon + 1);
    String day   = String(time_info.tm_mday);

    if (partial)
        display.setPartialWindow(0, 0, display.width(), display.height());
    else
        display.setFullWindow();
    display.firstPage();
    do {
        // 显示边框和分界线
        display.drawRect(16, 16, 617, 449, GxEPD_BLACK);    // 边框
        display.drawLine(16, 120, 632, 120, GxEPD_BLACK);   // 分割线

        display.drawLine(16, 134, 632, 134, GxEPD_BLACK);
        display.drawLine(16, 200, 632, 200, GxEPD_BLACK);
        display.drawLine(16, 266, 632, 266, GxEPD_BLACK);
        display.drawLine(16, 332, 632, 332, GxEPD_BLACK);
        display.drawLine(16, 398, 632, 398, GxEPD_BLACK);

        display.drawLine(104, 120, 104, 464, GxEPD_BLACK);
        display.drawLine(192, 120, 192, 464, GxEPD_BLACK);
        display.drawLine(280, 120, 280, 464, GxEPD_BLACK);
        display.drawLine(368, 120, 368, 464, GxEPD_BLACK);
        display.drawLine(456, 120, 456, 464, GxEPD_BLACK);
        display.drawLine(544, 120, 544, 464, GxEPD_BLACK);

        // 显示图片
        if (!partial) updatePic();
        display.drawBitmap(536, 24, pic[pic_index], 88, 88, GxEPD_BLACK, GxEPD_WHITE);   // 图像

        // 显示时间
        u8g2_fonts.setFont(CLOCK_FONT);
        u8g2_fonts.drawUTF8(24, 86, current_time);

        // 显示日期
        u8g2_fonts.setFont(DATE_FONT);
        u8g2_fonts.drawUTF8(
            25,
            110,
            String(year + "年" + month + "月" + day + "日 " + week[time_info.tm_wday]).c_str());

        // 显示电费
        display.drawBitmap(193, 27, icon_thermostat, 20, 20, GxEPD_BLACK, GxEPD_WHITE);
        display.drawBitmap(193, 49, icon_v_20px, 20, 20, GxEPD_BLACK, GxEPD_WHITE);
        display.drawBitmap(193, 71, icon_w_20px, 20, 20, GxEPD_BLACK, GxEPD_WHITE);
        display.drawBitmap(273, 27, icon_humidity_mid, 20, 20, GxEPD_BLACK, GxEPD_WHITE);
        display.drawBitmap(273, 49, icon_i_20px, 20, 20, GxEPD_BLACK, GxEPD_WHITE);
        display.drawBitmap(273, 71, icon_flash_on_20px, 20, 20, GxEPD_BLACK, GxEPD_WHITE);
        u8g2_fonts.setFont(ELECTRICITY_FONT);
        u8g2_fonts.drawUTF8(216, 64, (String(e_data.u) + "V").c_str());
        u8g2_fonts.drawUTF8(296, 64, (String(e_data.i) + "A").c_str());
        u8g2_fonts.drawUTF8(216, 86, (String(e_data.w) + "W").c_str());
        u8g2_fonts.drawUTF8(296, 86, (String(e_data.vol) + "度").c_str());

        // 显示温湿度
        u8g2_fonts.setFont(TEMPERATURE_FONT);
        u8g2_fonts.drawUTF8(214, 43, (String(t_data.temperature) + "℃").c_str());
        u8g2_fonts.drawUTF8(294, 43, (String(t_data.humidity) + "%").c_str());

        // 显示天气及预报天气

        u8g2_fonts.setFont(CITY_FONT);   // 城市字体
        if (!rt_weather.city.isEmpty()) {
            if (!rt_weather.weather.isEmpty())
                display.drawBitmap(369, 25, getWeatherIcon(), 40, 40, GxEPD_BLACK, GxEPD_WHITE);
            u8g2_fonts.drawUTF8(409, 40, rt_weather.city.c_str());
            u8g2_fonts.drawUTF8(
                409, 60, (rt_weather.weather + " " + rt_weather.temperature + "℃").c_str());
            u8g2_fonts.setFont(FORCAST_FONT);   // 预报字体
            u8g2_fonts.drawUTF8(371,
                                77,
                                (rt_weather.winddirection + "风 " + rt_weather.windpower +
                                 "级 湿度" + rt_weather.humidity + "%")
                                    .c_str());
            if (!forecast.tmr_dweather.equals(forecast.tmr_nweather))
                u8g2_fonts.drawUTF8(371,
                                    92,
                                    ("明天 " + forecast.tmr_dweather + "/" + forecast.tmr_nweather +
                                     " " + forecast.tmr_dtemp + "/" + forecast.tmr_ntemp + "℃")
                                        .c_str());
            else
                u8g2_fonts.drawUTF8(371,
                                    92,
                                    ("明天 " + forecast.tmr_dweather + " " + forecast.tmr_dtemp +
                                     "/" + forecast.tmr_ntemp + "℃")
                                        .c_str());
            if (!forecast.aft_dweather.equals(forecast.aft_nweather))
                u8g2_fonts.drawUTF8(371,
                                    107,
                                    ("后天 " + forecast.aft_dweather + "/" + forecast.aft_nweather +
                                     " " + forecast.aft_dtemp + "/" + forecast.aft_ntemp + "℃")
                                        .c_str());
            else
                u8g2_fonts.drawUTF8(371,
                                    107,
                                    ("后天 " + forecast.aft_dweather + " " + forecast.aft_dtemp +
                                     "/" + forecast.aft_ntemp + "℃")
                                        .c_str());
        }
        else { u8g2_fonts.drawUTF8(409, 40, "同步中"); }

    } while (display.nextPage());
}