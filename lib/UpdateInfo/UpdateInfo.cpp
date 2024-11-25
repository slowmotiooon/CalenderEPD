#include <Arduino.h>
#include <UpdateInfo.h>

extern tm     time_info;
extern String current_time;

bool needToUpdateTime() {
    return current_time[15] != asctime(&time_info)[15];
}

// 必须放在updatetime之后
bool needToUpdateDate(){
    return current_time.substring(8,10).equals(String(asctime(&time_info)).substring(8,10));
}