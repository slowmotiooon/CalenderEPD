#include <Arduino.h>
#include <Commons.h>
#include <UpdateInfo.h>

extern tm              time_info;
extern tm              old_time_info;
extern ElectricData    e_data;
extern ElectricData    old_e_data;
extern Temperature     t_data;
extern Temperature     old_t_data;
extern RealTimeWeather rt_weather;
extern Forecast        forecast;
extern bool                   partial;

bool needToUpdateInfo() {
    bool need;

    // 设置全局更新
    if(time_info.tm_hour%30 == 24) partial = false;

    // 更新时间
    need = time_info.tm_hour != old_time_info.tm_hour || time_info.tm_min != old_time_info.tm_min;
    if (need) return need;

    // 更新日期
    need = time_info.tm_mday != old_time_info.tm_mday || time_info.tm_mon != old_time_info.tm_mon ||
           time_info.tm_year != old_time_info.tm_year;
    if (need) return need;

    // 更新电量
    need = e_data.freeVol != old_e_data.freeVol || e_data.i != old_e_data.i ||
           e_data.u != old_e_data.u || e_data.vol != old_e_data.vol;
    if (need) return need;

    return need;
}

bool needToUpdateElec() {
    return time_info.tm_min % ELEC_FREQ == 0 && time_info.tm_sec == 10;
}

bool needToUpdateWeather() {
    if (rt_weather.city.isEmpty()) { return time_info.tm_sec == 20; }
    else
        return time_info.tm_min % WEATHER_FREQ == 1 && time_info.tm_sec == 20;
}

bool needToUpdateForecast() {
    if (forecast.aft_dtemp.isEmpty()) { return time_info.tm_sec == 20; }
    else
        return time_info.tm_min % FORECAST_FREQ == 1 && time_info.tm_sec == 20;
}