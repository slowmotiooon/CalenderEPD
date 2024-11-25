#include <EpdDisplay.h>
#include <Arduino.h>
#include <Commons.h>
#include "GxEPD2_BW.h"
#include "epd/GxEPD2_583_T8.h"
#include <U8g2_for_Adafruit_GFX.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <air.c>
#include <humidity_mid.c>
#include <thermostat.c>
#include <i_20px.c>
#include <v_20px.c>
#include <w_20px.c>
#include <flash_on_20px.c>
#include <p1.c>

SPISettings epd_spi_settings;
SPIClass epd_spi;

GxEPD2_BW<GxEPD2_583_T8, GxEPD2_583_T8::HEIGHT / 2> display(
    GxEPD2_583_T8(/*CS*/ EPD_CS, /*DC*/ EPD_DC, /*RST*/ EPD_RST, /*BUSY*/ EPD_BUSY));

U8G2_FOR_ADAFRUIT_GFX u8g2_fonts;

extern tm time_info;

extern char hhmm[6];

void displayInit() {
    epd_spi.begin(EPD_CLK, EPD_MISO, EPD_MOSI, EPD_CS);
    epd_spi_settings = SPISettings();
    display.init(115200, true, 2, false, epd_spi, epd_spi_settings);
    display.setRotation(4);
    u8g2_fonts.begin(display);
    u8g2_fonts.setFontMode(1);
    u8g2_fonts.setFontDirection(0);
    u8g2_fonts.setForegroundColor(GxEPD_BLACK);
    u8g2_fonts.setBackgroundColor(GxEPD_WHITE);
}

void displayTimeTest() {
    display.setPartialWindow(0,0,128,24);
    display.setTextColor(GxEPD_BLACK);
    display.setFont(&FreeMonoBold9pt7b);
    display.firstPage();
    do{
        display.setCursor(0, 20);
        display.print(hhmm);
    } while(display.nextPage());
}

void displayBackground() {
    display.setFullWindow();
    display.firstPage();
    do{
        display.fillScreen(GxEPD_WHITE);
        display.drawRect(16,16,617,449,GxEPD_BLACK);    //边框
        display.drawLine(16,120,632,120,GxEPD_BLACK);   //分割线

        display.drawLine(16,134,632,134,GxEPD_BLACK);
        display.drawLine(16,200,632,200,GxEPD_BLACK);
        display.drawLine(16,266,632,266,GxEPD_BLACK);
        display.drawLine(16,332,632,332,GxEPD_BLACK);
        display.drawLine(16,398,632,398,GxEPD_BLACK);

        display.drawLine(104,120,104,464,GxEPD_BLACK);
        display.drawLine(192,120,192,464,GxEPD_BLACK);
        display.drawLine(280,120,280,464,GxEPD_BLACK);
        display.drawLine(368,120,368,464,GxEPD_BLACK);
        display.drawLine(456,120,456,464,GxEPD_BLACK);
        display.drawLine(544,120,544,464,GxEPD_BLACK);

        display.drawRect(24,24,160,64,GxEPD_BLACK);     //时间
        u8g2_fonts.setFont(CLOCK_FONT);                 //时间字体
        u8g2_fonts.drawUTF8(24,86, "22:07");
        display.drawRect(24,92,160,20,GxEPD_BLACK);     //日期
        u8g2_fonts.setFont(DATE_FONT);                  //日期字体
        u8g2_fonts.drawUTF8(25,110, "2021年06月01日 星期二");

        display.drawRect(192,24,168,88,GxEPD_BLACK);    //电量和温湿度
        display.drawBitmap(193,27,icon_thermostat,20,20,GxEPD_BLACK,GxEPD_WHITE);
        display.drawBitmap(193, 49, icon_v_20px, 20, 20, GxEPD_BLACK, GxEPD_WHITE);
        display.drawBitmap(193, 71, icon_w_20px, 20, 20, GxEPD_BLACK, GxEPD_WHITE);
        display.drawBitmap(273, 27, icon_humidity_mid, 20, 20, GxEPD_BLACK, GxEPD_WHITE);
        display.drawBitmap(273,49,icon_i_20px,20,20,GxEPD_BLACK,GxEPD_WHITE);
        display.drawBitmap(273, 71, icon_flash_on_20px, 20, 20, GxEPD_BLACK, GxEPD_WHITE);
        u8g2_fonts.setFont(TEMPERATURE_FONT);   // 温湿度字体
        u8g2_fonts.drawUTF8(214, 44, "25℃");
        u8g2_fonts.drawUTF8(294,44,"50%");
        u8g2_fonts.drawUTF8(214, 66, "220.0V");
        u8g2_fonts.drawUTF8(294, 66, "10.0A");
        u8g2_fonts.drawUTF8(214, 88, "2200.0W");
        u8g2_fonts.drawUTF8(294, 88, "127.45度");
        u8g2_fonts.drawUTF8(194,110,"第48周, 教学周第14周");

        display.drawRect(368,24,160,88,GxEPD_BLACK);     //天气
        display.drawRect(368,24,40,40,GxEPD_BLACK);     //天气图标
        display.drawBitmap(369,25,icon_air,40,40,GxEPD_BLACK,GxEPD_WHITE);
        u8g2_fonts.setFont(CITY_FONT);                  //城市字体
        u8g2_fonts.drawUTF8(409,40,"海淀区");
        u8g2_fonts.drawUTF8(409,60,"晴 25℃");
        u8g2_fonts.setFont(FORCAST_FONT);               //预报字体
        u8g2_fonts.drawUTF8(371, 77, "西北风 5级");
        u8g2_fonts.drawUTF8(371, 92, "明天 中到大雪 -13/-11℃");
        u8g2_fonts.drawUTF8(371, 107, "后天 小雨 13/17℃");

        display.drawBitmap(536,24,pic_p1,88,88,GxEPD_BLACK,GxEPD_WHITE);    //图像

        u8g2_fonts.setFont(CALENDAR_WEEK_FONT);               //日历星期字体
        u8g2_fonts.drawUTF8(48, 132, "周日");
        u8g2_fonts.drawUTF8(136, 132, "周一");
        u8g2_fonts.drawUTF8(224, 132, "周二");
        u8g2_fonts.drawUTF8(312, 132, "周三");
        u8g2_fonts.drawUTF8(400, 132, "周四");
        u8g2_fonts.drawUTF8(488, 132, "周五");
        u8g2_fonts.drawUTF8(576, 132, "周六");


        display.drawRect(24,142,72,50,GxEPD_BLACK);     //日历项边框
        display.drawRect(24,142,40,50,GxEPD_BLACK);
        u8g2_fonts.setFont(CALENDAR_NUM_FONT);          //日历字体
        u8g2_fonts.drawUTF8(25, 172, "27");
        u8g2_fonts.setFont(CALENDAR_DAY_FONT);          //日历字体
        u8g2_fonts.drawUTF8(25,186, "中秋节");
        u8g2_fonts.setFont(CALENDER_EVENT_FONT);        //日历字体
        u8g2_fonts.drawUTF8(68, 150, "10:00");
        u8g2_fonts.drawUTF8(68,158,"11:25");
        u8g2_fonts.drawUTF8(68,166,"13:00");
        u8g2_fonts.drawUTF8(68, 174, "14:40");
        u8g2_fonts.drawUTF8(68,182,"15:35");
        u8g2_fonts.drawUTF8(68,190,"16:25");


    } while(display.nextPage());
}

void displayUpdateAll(bool partial){
    if(partial) display.setPartialWindow(whole_screen.x,whole_screen.y,whole_screen.w,whole_screen.h);
    else display.setFullWindow();
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

    } while(display.nextPage());
    display.hibernate();
}

void displayUpdateTime(){
    display.setPartialWindow(time_window.x,time_window.y,time_window.w,time_window.h);
    display.firstPage();
    do{
        u8g2_fonts.setFont(CLOCK_FONT);
        u8g2_fonts.drawUTF8(24,86,hhmm);
    } while(display.nextPage());
    display.hibernate();
}